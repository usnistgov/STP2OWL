#!/usr/bin/env python3

import logging
import os
import csv
import sys

if len(sys.argv) < 3 :
    logging.error("Usage: ".sys.argv[0] + "[profile] [ontology]")

profile = os.getcwd() + "\\Profiles\\" + sys.argv[1]
ontology = sys.argv[2]

# open file with profile specification
try :
    profileFile = open(profile, "r")
except BaseException as e :
    logging.error("File ".format(profile) + " couldn't be opened.")

profiles = []
# read profile specification
profileReader = csv.reader(profileFile)

for rr in profileReader :
    p = []
    for r in rr :
        try :
            value = int(r)
        except BaseException as e:
            value = r
        p += [value]
    profiles += [p]

# put profile specification into a table
# while profileReader :
#     profiles.append(profileReader.strip())
#     profileLine = profileFile.readline()

# profileFile.close()

# with open(profile, newline='') as profileFile :
#     profiles = csv.DictReader(profileFile)


try :
    ontologyFile = open(ontology, "r")
except BaseException as e :
    logging.error("File ".format(ontology) + " couldn't be opened.")

newOntology = ""
ontologyLine = ontologyFile.readline()

# print(profiles)
while ontologyLine :
    ips = 0
    isOk = 0
    for ps in profiles :
        if ips > 0 and ontologyLine.find(ps[0]) >= 0 :
            # ip = 0
            for k,p in enumerate(ps) :
                if p in (0,1) :
                    if p > 0 and ontologyLine.find(profiles[0][k]) >= 0 :
                        isOk += 1
                    # if ps[0].find('int') :
                        # print("line: " + ontologyLine + " features:" + ps[0] + " " + profiles[0][k] + ":" + str(p) + " isOk=" + str(isOk))
                        # print(str(p > 0 and ontologyLine.find(profiles[0][k]) >= 0))
                # ip += 1
            if isOk <= 0 :
                newOntology += "# "
                break
        ips += 1
    newOntology += ontologyLine
    ontologyLine = ontologyFile.readline()

profile = sys.argv[1]

sfn = os.path.splitext(ontologyFile.name)
newOntologyFileName = sfn[0] + "_" + profile + sfn[1]
print(newOntologyFileName)

try :
    newOntologyFile = open(newOntologyFileName, "x")
except BaseException as e :
    logging.error("File ".format(newOntologyFileName) + " couldn't be created.")

newOntologyFile.write(newOntology)



