# NIST STEP to OWL Translator (STP2OWL)
The NIST STEP to OWL Translator (STP2OWL) is an open-source software, and an improved implementation of [OntoSTEP](https://www.nist.gov/services-resources/software/ontostep-plugin). STP2OWL translates STEP schemas (EXPRESS) and instance files (P21) into Web Ontology Language (OWL) files in a faster and more flexible way, thus furthering the adoption of the full capabilities of ISO 10303. Developed at the National Institute of Standards and Technology (NIST), the software is based on the [STEPcode](https://github.com/stepcode/stepcode) and written in C++.
![Components and data flow of the STP2OWL(OntoSTEP)](https://user-images.githubusercontent.com/67339153/181289824-56662e6d-40d7-450c-9376-37873e9858e5.png "Components and data flow of the STP2OWL(OntoSTEP)")

## Prerequisites
- [STEPcode](https://github.com/stepcode/stepcode) v0.9
  - You must build the STEPcode solution first to generate DLL files.
  - The [current release](https://github.com/usnistgov/STP2OWL/releases/tag/v1.0) was built on Visual Studio 2019 (x64).
  - The complete set of DLL files required for the current STP2OWL to run is as follows.
    - STEPcode related: stepdai.dll, stepcore.dll, stepeditor.dll, steputils.dll, sdai_ap203.dll, sdai_ap203e2.dll, sdai_ap209.dll, sdai_ap210e2.dll, sdai_ap210e3.dll, sdai_ap214e3.dll, sdai_ap219.dll, sdai_ap227.dll, sdai_ap235.dll, sdai_ap238.dll, sdai_ap239.dll, sdai_ap240.dll, sdai_ap242.dll, sdai_ap242e2.dll, sdai_ifc2x3.dll, sdai_IFC4.dll, sdai_ISO15926.dll, sdai_pdm.dll
    - Users can generate new DLLs for new EXPRESS schemas.

## Build the STEP to OWL Translator
- Unzip the "stepcode.zip" to where the VS solution file (STP2OWL.sln) is located.
- Create a "stepcode\bin" folder and build the stepcode in that folder using CMake (See [here](https://github.com/stepcode/stepcode/blob/master/INSTALL) for more details).
  - If you want to build a new schema, add it to "stepcode\data" folder beore CMake build.
- Build all the projects in "stepcode\bin\SC.sln".
- Unzip the "schemas.zip" to "stepcode\bin" which will add two files (.cc and .h) for each schema folder in "stepcode\bin\schemas".
- Open "SC.sln" and add unziped files to the corresponding VS project for each schema.
- Rebuild all the projects in "stepcode\bin\SC.sln".
- Build "STP2OWL.sln".

## Usage
- NIST STP2OWL is a command line software. Please check out the [Usage guide](USAGE.md).

## Contact Information
- Soonjo Kwon, soonjo.kwon@kumoh.ac.kr
- William Z. Bernstein, william.bernstein@afresearchlab.com
- Laetitia V. Monnier, lve.monnier@gmail.com

## Version
1.0

## Reference
- Soonjo Kwon, Laetitia V. Monnier, Raphael Barbau, William Z. Bernstein, 2022, [**A New Implementation of OntoSTEP: Flexible Generation of Ontology and Knowledge Graphs of EXPRESS-Driven Data**](https://doi.org/10.1115/1.4053079), *ASME Journal of Computing and Information Science in Engineering*, 22(2), 024502.

## Disclaimers
[NIST Disclaimer](https://www.nist.gov/disclaimer)


