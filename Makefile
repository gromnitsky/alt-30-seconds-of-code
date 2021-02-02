$(if $(i),,$(error i= param is missing))
o := _out
# supported values: js, css, react
t := js

all: $(o)/index.html

$(o)/%.html: $(i)/%.md
	$(mkdir)
	echo '<article id="$(id)">' > $@
	echo '<h2>$(title)</h2>' >> $@
	pandoc $< -t html --no-highlight >> $@
	echo '</article>' >> $@

id = $(notdir $(basename $<))
title = $(shell ruby -ryaml -e 'puts YAML.load($$<)["title"]' $<)

$(o)/index.html: template.html $(patsubst $(i)/%.md, $(o)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat $^ | erb date=`git -C $(dir $(i)) log -1 --format=%cI`  t=$(t) > $@
	echo '</main>' >> $@

$(o)/node_modules/%: node_modules/%
	$(mkdir)
	cp $< $@

ifeq ($(t),react)
all: $(o)/node_modules/@babel/standalone/babel.min.js \
	$(o)/node_modules/react/umd/react.production.min.js \
	$(o)/node_modules/react-dom/umd/react-dom.production.min.js
endif

.DELETE_ON_ERROR:
mkdir = @mkdir -p $(dir $@)

upload: $(o)/index.html
	rsync -av --progress $< alex@sigwait.tk:'~/public_html/demo/misc/30-seconds-of-code/$(notdir $(o))/'
