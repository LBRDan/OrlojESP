# Generate index.html from templates/index.html using generateIndexHtml.mjs

DASEL := $(shell command -v dasel 2> /dev/null)

all:
ifndef DASEL
    $(error "dasel is not available please install dasel")
endif
indexgen:
	fnm use
	dasel -r csv -w json -f tz.csv | ./generateIndexHtml.mjs

