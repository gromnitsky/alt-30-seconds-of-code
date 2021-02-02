# An alternative interface to '30 seconds of code'

* [css](http://sigwait.tk/~alex/demo/misc/30-seconds-of-code/css/)
* [react](http://sigwait.tk/~alex/demo/misc/30-seconds-of-code/react/)
* [javascript](http://sigwait.tk/~alex/demo/misc/30-seconds-of-code/js/)

Why & how: http://gromnitsky.blogspot.com/2021/01/fixing-30-seconds-of-code.html

## Reqs

* pandoc
* ruby

## Compilation

1. Clone one of the supported repos, for example, about css:

        $ git clone https://github.com/30-seconds/30-seconds-of-css

2. Clone this repo, cd to it, fetch optional deps:

        $ npm i

3. Generate `_out/css/index.html`:

        $ make i=../30-seconds-of-css/snippets o=_out/css t=css

## License

MIT
