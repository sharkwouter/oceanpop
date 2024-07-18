# Contributing

Thank you for considering contributing to OceanPop.

There are many ways to contribute to OceanPop, for example you can submit bug reports, write code or modify code comments to make them more clear.

When contributing, please follow this guidelines:

* For major changes, please open an issue first.
* Use `snake_case` for variable, function and method names, `PascalCase` for class and struct names and `UPPER_SNAKE_CASE` for constants and macros.
* Use 4 spaces for indentation.
* Use references instead of pointers in function arguments when possible.

## First contribution.

Don't know where to start? You can open our issue tracker and search for issues with 'good first issue' label and try to fix them.

## Submitting a bug report

When submitting a bug report, please make sure to include this information:
* What OS are you using?
* Steps to reproduce.

## Adding a new translation

By running cmake with the `-DTRANSLATION_SUPPORT=1` option, translation support can be enabled. To add a new language, for example German, you would run the following command:

```
platform/add-translation.sh de
```

In this case `de` is the code the the German language. A `.po` file will be created in `assets/languages` for the new language. This file can be edited with poedit or a text editor. Feel free to add you language and make a pull request.