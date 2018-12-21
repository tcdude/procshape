# Development Guidelines

## Code

* snake case
* docstring for classes with description and \_\_init\_\_ args, where applicable
* docstring for methods and functions with additional "# type: (arg_type, ...) -> *return_type*" declaration


## Git

### Branches

* **master** 
    * latest stable version
    * major version tags
* **develop**
    * current development version
    * based on ``master``
    * merges into ``master`` with new major version tag
    * has minor version tag
* **_issue_-branches**
    * issue branch for feature development 
    * based on ``develop``
    * merges into ``develop`` with new micro version tag
* **_hot_fix_-branches**
    * hot-fix branch for implementing critical bug fixes
    * based on ``master`` and respective version tag
    * merges into ``master`` with new micro version tag
    * merges into ``develop`` with current micro version tag
    
### Versioning

* major = merge year
* minor = merge month
* micro = incremental number = total merges