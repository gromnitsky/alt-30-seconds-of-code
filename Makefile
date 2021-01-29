$(if $(i),,$(error i= param is missing))
out := _out

$(out)/%.html: $(i)/%.md
	@mkdir -p $(dir $@)
	pandoc $< -t html5 -s --no-highlight | adieu -pe '$$("header").attr("id", null); $$("body").html()' > $@

$(out)/30-seconds-of-code.html: $(patsubst $(i)/%.md, $(out)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat template.html $^ <(echo '</main>') > $@

SHELL := bash -o pipefail
.DELETE_ON_ERROR:
