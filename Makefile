$(if $(i),,$(error i= param is missing))
out := _out

$(out)/%.html: $(i)/%.md
	@mkdir -p $(dir $@)
	echo '<h2 id="$(title)">$(title)</h2>' > $@
	pandoc $< -t html --no-highlight >> $@

title = $(notdir $(basename $@))

$(out)/30-seconds-of-code.html: template.html $(patsubst $(i)/%.md, $(out)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat $^ > $@
	echo '</main>' >> $@

.DELETE_ON_ERROR:

upload: $(out)/30-seconds-of-code.html
	rsync -av $< alex@sigwait.tk:'~/public_html/demo/misc/'
