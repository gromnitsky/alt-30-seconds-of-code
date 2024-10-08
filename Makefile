$(if $(i),,$(error i= param is missing))
o := _out
# supported values: js, css, react
t := js

all: $(o)/index.html

$(o)/%.html: $(i)/%.md
	$(mkdir)
	echo '<article id="$(id)">' > $@
	$(if $(title),echo '<h2>'$(call se,$(title))'</h2>' >> $@)
	cd $(dir $<) && erb -r $(CURDIR)/lib.rb  $(notdir $<) | pandoc -t html --no-highlight >> $(CURDIR)/$@
	echo '</article>' >> $@

id = $(notdir $(basename $<))
title = $(shell awk '/^---/ {n++} {print $$0; if (n>1) exit(0)}' $< | js-yaml | json 0 | json title)

$(o)/index.html: template.html $(patsubst $(i)/%.md, $(o)/%.html, $(sort $(wildcard $(i)/*.md)))
	cat $^ | erb date=`git -C $(dir $(i)) log -1 --format=%cI`  t=$(t) > $@
	echo '</main>' >> $@

$(o)/node_modules/%: node_modules/%
	$(mkdir)
	cp $< $@

ifeq ($(t),react)
deps := $(o)/node_modules/@babel/standalone/babel.min.js \
	$(o)/node_modules/react/umd/react.production.min.js \
	$(o)/node_modules/react-dom/umd/react-dom.production.min.js
endif
all: $(deps)

SHELL := bash -o pipefail
.DELETE_ON_ERROR:
mkdir = @mkdir -p $(dir $@)
se = '$(subst ','\'',$1)'

upload: $(o)/index.html $(if $(deps),$(o)/node_modules)
	rsync -av --progress $^ gromnitsky@web.sourceforge.net:/home/user-web/gromnitsky/htdocs/articles/alt30soc/$(notdir $(o))/
