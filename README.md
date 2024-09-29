# An alternative interface to '30 seconds of code'

* [CSS](http://gromnitsky.users.sourceforge.net/articles/alt30soc/css/)
* [React](http://gromnitsky.users.sourceforge.net/articles/alt30soc/react/)
* [JavaScript](http://gromnitsky.users.sourceforge.net/articles/alt30soc/js/)
* [Plain C](http://gromnitsky.users.sourceforge.net/articles/alt30soc/c/)

Why & how: https://sigwait.org/~alex/blog/2021/01/30/fixing-30-seconds-of-code.html

## Reqs

* pandoc
* ruby
* `npm i -g js-yaml json`

## Compilation

    $ npm i

Generate `_out/css/index.html`:

    $ make i=css/snippets o=_out/css t=css

Replace `css` with `c`, `js`, or `react`.

## License

MIT, including "Plain C" snippets.

`js/`, `css/`, and `react/` dirs are from the defunct or "archived"
repos. All 3 were licensed under the CC0-1.0 License.
