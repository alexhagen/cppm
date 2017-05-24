all: doxygen breathe publish

doxygen: FORCE
	doxygen docs/source/Doxyfile

breathe: FORCE
	pandoc README.md -o docs/source/README.rst; \
	jupyter nbconvert examples/example_shm.ipynb --to html --template basic --execute; \
	mv examples/example_shm.html docs/source/README_SHM.html; \
	jupyter nbconvert examples/example_value.ipynb --to html --template basic --execute; \
	mv examples/example_value.html docs/source/README_VALUE.html; \
	cd docs/; \
	sphinx-apidoc -e -f -M -o ./ ../; \
	make html

publish: FORCE
	mkdir -p ~/pages/cppm; \
	cd ~/pages/cppm; \
	git rm -r *; \
	cd ~/code/cppm/docs; \
	cp -r build/html/* ~/pages/cppm; \
	cd ~/pages/cppm; \
	git add *; \
	touch .nojekyll; \
	git add .nojekyll; \
	git commit -am "$(shell git log -1 --pretty=%B | tr -d '\n')"; \
	git push origin gh-pages; \
	cd ~/code/cppm

FORCE:
