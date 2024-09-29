# An alternative interface to '30 seconds of code'

* [css](http://gromnitsky.users.sourceforge.net/articles/alt30soc/css/)
* [react](http://gromnitsky.users.sourceforge.net/articles/alt30soc/react/)
* [javascript](http://gromnitsky.users.sourceforge.net/articles/alt30soc/js/)

Snippets from this repo:

* [Plain C](http://gromnitsky.users.sourceforge.net/articles/alt30soc/c/)

Why & how: https://sigwait.org/~alex/blog/2021/01/30/fixing-30-seconds-of-code.html

## Reqs

* pandoc
* ruby
* `npm i -g js-yaml`

## Compilation

1. Clone one of the supported repos, for example, about css:

        $ git clone https://github.com/30-seconds/30-seconds-of-css

2. Clone this repo, cd to it, fetch optional deps:

        $ npm i

3. Generate `_out/css/index.html`:

        $ make i=../30-seconds-of-css/snippets o=_out/css t=css

## License

MIT, including "Plain C" snippets.

`js/`, `css/`, and `react/` dirs are from the defunct or "archived"
repos. All 3 were licensed under the CC0-1.0 License.
