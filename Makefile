all: doxygen breathe

doxygen: FORCE
	doxygen docs/source/Doxyfile

breathe: FORCE
	cd docs/; \
	sphinx-apidoc -e -f -M -o ./ ../; \
	make html

FORCE:
