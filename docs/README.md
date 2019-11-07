# Documentation

To Build the documentation:

## Requirments:

- [Sphinx](http://www.sphinx-doc.org/en/master/) - Python documentation generator
- [Read the Docs Sphinx Theme](https://sphinx-rtd-theme.readthedocs.io/en/stable/) - Theme for final output
- [recommonmark](https://github.com/miyakogi/m2r) - Markdown to reStructuredText


```
pip install sphinx sphinx_rtd_theme recommonmark
```

## Building

```bash
make html
```

Or install Sphinx-Serve
```bash
pip install sphinx-serve
```

and run
```bash
sphinx-serve
```


## Read the docs

The documentation is available at https://g4e.readthedocs.io

It updates automatically when changes are pushed do the repo

