# NIST STEP to OWL Translator (STP2OWL)
The NIST STEP to OWL Translator (STP2OWL) is is an open-source software, and an improved implementation of [OntoSTEP](https://www.nist.gov/services-resources/software/ontostep-plugin). STP2OWL translates STEP schemas (EXPRESS) and instance files (P21) in a faster and more flexible way, thus furthering the adoption of the full capabilities of ISO 10303. Developed at the National Institute of Standards and Technology (NIST), the software is based on the [STEPcode](https://github.com/stepcode/stepcode) and written in C++.

## Prerequisites
- [STEPcode](https://github.com/stepcode/stepcode) v0.9
  - You must build the STEPcode solution first to generate DLL files.
  - The complete set of DLL files required for the current STP2OWL to run is as follows.
    - STEPcode related: stepdai.dll, stepcore.dll, stepeditor.dll, steputils.dll, sdai_ap203.dll, sdai_ap203e2.dll, sdai_ap209.dll, sdai_ap210e2.dll, sdai_ap210e3.dll, sdai_ap214e3.dll, sdai_ap219.dll, sdai_ap227.dll, sdai_ap235.dll, sdai_ap238.dll, sdai_ap239.dll, sdai_ap240.dll, sdai_ap242.dll, sdai_ap242e2.dll, sdai_ifc2x3.dll, sdai_IFC4.dll, sdai_ISO15926.dll, sdai_pdm.dll
    - Users can generate new DLLs for new EXPRESS schemas.

## Build the STEP to OWL Translator
- Unzip the "stepcode.zip" to where the VS solution file (STP2OWL.sln) is located.
- Create a "bin" folder and build the stepcode in that folder using CMake (See [here](https://github.com/stepcode/stepcode/blob/master/INSTALL) for more details).
- Build all the projects in "stepcode\bin\SC.sln".
- Unzip the "schemas.zip" to "stepcode\bin" which will add two files (.cc and .h) for each schema folder in "stepcode\bin\schemas".
- Open "SC.sln" and add unziped files for each schema to the corresponding VS project.
- Rebuild all the projects in "stepcode\bin\SC.sln".
- Build "STP2OWL.sln".

## Usage
- NIST STP2OWL is a command line software. Please check out the [Usage guide](USAGE.md).

## Contact Information
- Soonjo Kwon, soonjo.kwon@kumoh.ac.kr
- William Z. Bernstein, william.bernstein@nist.gov

## Version
1.0

## Disclaimers
[NIST Disclaimer](https://www.nist.gov/disclaimer)


