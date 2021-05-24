# Usage Guide
The following information describes how to use the software. Running the execution file in the command line prompt will also give you the instruction.

## How to use in the command line prompt
- STP2OWL.exe option1 value1 option2 value2 ..

## Options
- --input: Input STEP file path
- --schema: STEP schema (Available input:203,203e2,210e2,210e3,214e3,219,227,235,238,239,240,242,242e2,IFC2X3,IFC4,ISO15926,PDM)
- --profile: OWL 2 profile (Available input:DL,EL,QL,RL) default=DL
- --mode: Output mode (0:schema,instance, 1:instance, 2:schema, 3:integrated) default=0
- --simgeom: Simplified geometry representation (1:yes, 0:no) default=0
- --filter: CSV file path for STEP entity names to be kept in the OWL instances

## Examples
- STP2OWL.exe --input Model.stp --profile RL --simgeom 1
- STP2OWL.exe --mode 1 --input Model.step --profile EL --filter target.csv
- STP2OWL.exe --schema 210e2 --profile QL
