$(if $(i),,$(error i= param is missing))
o := _out
# supported values: js, css
t := js

$(o)/%.html: $(i)/%.md
	@mkdir -p $(dir $@)
	echo '<article id="$(title)">' > $@
	echo '<h2>$(title)</h2>' >> $@
	pandoc $< -t html --no-highlight >> $@
	echo '</article>' >> $@

title = $(notdir $(basename $<))

$(o)/index.html: template.html $(patsubst $(i)/%.md, $(o)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat $^ | erb t=$(t) > $@
	echo '</main>' >> $@

.DELETE_ON_ERROR:

upload: $(o)/index.html
	rsync -av $< alex@sigwait.tk:'~/public_html/demo/misc/30-seconds-of-code/$(notdir $(o))/'
