let http = require('http')
let url = require('url')
let qs = require('querystring')
let fs = require('fs')
let multiparty = require('multiparty')

function concat_stream(s) {
    let chunks = []
    return new Promise( (resolve, reject) => {
        s.on('data', chunk => chunks.push(chunk))
        s.on('error', err => reject(err))
        s.on('end', () => resolve(Buffer.concat(chunks).toString('utf8')))
    })
}

let server = http.createServer(function (req, res) {
    let log = console.error.bind(console, `${req.method} ${req.url}:`)
    let err = (code, msg) => {
        try { res.statusCode = code } catch (e) { log(code, e.message) }
        log(code, msg || '')
        res.end()
    }
    let u = url.parse(req.url, true)

    if (req.method === "GET" & u.pathname === '/form1') {
        res.setHeader('Content-Type', 'text/html')
        res.end(`<!doctype html>
<form action='/form1' method='post'
 style='max-width: 300px; display: grid; grid-template-columns: 20% 80%; row-gap: 5px'>
Name: <input name='name' value='${process.env.USER}'>
Age: <input type='number' name='age' value="100">
<input type='submit'>
</form>`)

    } else if (req.method === "GET" & u.pathname === '/form1/success') {
        res.setHeader('Content-Type', 'application/json')
        res.end(JSON.stringify(u.query))

    } else if (req.method === 'POST' & u.pathname === '/form1') {
        concat_stream(req).then( r => {
            res.writeHead(303, {'Location': `/form1/success?${r}`})
            res.end()
        }).catch( e => err(500, e.message))

    } else if (req.method === "GET" & u.pathname === '/form2') {
        res.setHeader('Content-Type', 'text/html')
        res.end(`<!doctype html>
<form action='/form2' method='post' enctype="multipart/form-data"
 style='max-width: 300px; display: grid; grid-template-columns: 20% 80%; row-gap: 5px'>
Name: <input name='name' value='${process.env.USER}'>
Photo: <input type='file' name='photo'>
<input type='submit'>
</form>`)

    } else if (req.method === "GET" & u.pathname === '/form2/success') {
        res.setHeader('Content-Type', 'text/html')
        res.end(`<!doctype html>
<h1>${u.query.name}</h1>
<img src='/${u.query.photo}'>
`)

    } else if (req.method === "GET" & /^\/tmp\//.test(u.pathname)) {
        res.setHeader('Content-Type', 'image/webp')
        let photo = fs.createReadStream(u.pathname.slice(1))
        photo.on('error', e => err(500, `${err.syscall} ${e.code}`))
        photo.pipe(res)

    } else if (req.method === 'POST' & u.pathname === '/form2') {
        fs.mkdirSync('tmp', {recursive:true})
        let form = new multiparty.Form({uploadDir: 'tmp'})
        form.parse(req, function(e, fields, files) {
            if (e) {
                err(500, e.message)
                return
            }
            let r = {name: fields.name, photo: files.photo[0].path }
            res.writeHead(303, {'Location': '/form2/success?'+qs.stringify(r)})
            res.end()
        })
    } else
        err(400)
})

server.listen(process.env.HOST || 3000, () => {
    console.error(`http://127.0.0.1:${server.address().port}`)
}).on('error', e => {
    console.error(`Error: ${e.message}`)
    process.exit(1)
})
