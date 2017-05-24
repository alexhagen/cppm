all: doxygen breathe

doxygen: FORCE
	doxygen docs/source/Doxyfile

breathe: FORCE
	pandoc README.md -o docs/source/README.rst; \
	jupyter nbconvert examples/example_shm.ipynb --to html --template basic --execute; \
	mv examples/example_shm.html docs/source/README_SHM.html; \
	pandoc readmes/readme_cll.md -o docs/source/README_CLL.rst; \
	pandoc readmes/readme_ah_value.md -o docs/source/README_VALUE.rst; \
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
