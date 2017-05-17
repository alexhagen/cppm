all: doxygen breathe

doxygen: FORCE
	doxygen docs/source/Doxyfile

breathe: FORCE
	pandoc README.md -o docs/source/README.rst; \
	pandoc readmes/readme_cll.md -o docs/source/README_CLL.rst; \
	pandoc readmes/readme_ah_value.md -o docs/source/README_VALUE.rst; \
	cd docs/; \
	sphinx-apidoc -e -f -M -o ./ ../; \
	make html

FORCE:
