import re
import fileinput
data = '';
for line in fileinput.input():
    if not line.startswith('@path'):
        data += line;
p = re.compile(ur'(?<=\\text\{)(.*)\\ensuremath\{(.*)\}(.*)(?=\})')
#test_str = u"$$\ensuremath{r} \underbrace{\frac{D}{Dt}\intop_{V_{m}}\psi dV}_{\substack{\text{change of \ensuremath{\psi}}\\\n\text{per unit time}\\\n\text{in volume ***V_{m}***}\n}\n}=-\underbrace{\oint_{S_{m}}\boldsymbol{J}\cdot\vec{n}dS}_{\substack{\text{influx of \ensuremath{\psi}across}\\\n\text{surface \ensuremath{S_{m}}}\n}\n}+\underbrace{\int_{V_{m}}\dot{\psi_{g}}dV}_{\substack{\text{generation of \ensuremath{\psi}}\\\n\text{in volume \ensuremath{V_{m}}}\n}\n}$$\n"
subst = r"\1$\2$\3"
  
result = re.sub(p, subst, data)

print result