$(if $(i),,$(error i= param is missing))
out := _out

$(out)/%.html: $(i)/%.md
	@mkdir -p $(dir $@)
	pandoc $< -t html5 -s --no-highlight | adieu -pe '$$("header").attr("id", $$("header>h1").text()); $$("body").html()' > $@

$(out)/30-seconds-of-code.html: template.html $(patsubst $(i)/%.md, $(out)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat $^ <(echo '</main>') > $@

SHELL := bash -o pipefail
.DELETE_ON_ERROR:

upload: $(out)/30-seconds-of-code.html
	rsync -av $< alex@sigwait.tk:'~/public_html/demo/misc/'
