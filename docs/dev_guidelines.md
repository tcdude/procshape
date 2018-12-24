# Development Guidelines

## Code

* PEP 8 for code
* PEP 257 docstrings
* docstring for packages in \_\_init\_\_.py
* docstring for modules
* docstring for classes with description and \_\_init\_\_ args, where applicable
* docstring for methods and functions 
* typing used with "# type: (arg_type, ...) -> *return_type*" declaration
* PyCharm shows me a green check mark on every file


## Git

### Branches

Simple structure for one dev: 

* **master** 
    * latest stable version
    * major and minor version tags on release
* **develop**
    * current development version
    * based on ``master``
    * merges into ``master`` with new major version tag
    
### Versioning

* major = merge year
* minor = merge month
