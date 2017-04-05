#!/usr/bin/env python

import math

# open LUT file for writing
print "Creating tower Et threshold LUT with filename 'lut_etThresh_2017v1.txt'"
towEtThreshLUTFile = open("L1Trigger/L1TCalorimeter/data/lut_etThresh_2017v1.txt", "w")

# write header info
towEtThreshLUTFile.write(
    "# address to et sum tower Et threshold LUT\n" +
    "# maps 16 bits to 8 bits\n" +
    "# 16 bits = (nTT4<<6) + abs(ieta)\n" +
    "# nTT4 is unsigned 10 bits, abs(ieta) is 6 bits\n" +
    "# anything after # is ignored with the exception of the header\n" +
    "# the header is first valid line starting with #<header> versionStr nrBitsAddress nrBitsData </header>\n" +
    "#<header> v1 16 8 </header>\n"

)

# vector of calo tower widths in eta, relative to central barrel widths (0.087 in eta)
# excludes ieta=0 and dummy ieta=29

towerEtaWidths = [1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
                  1.,1.,1.,1.,1.,1.,1.,1.,1.,1.,
                  1.03,1.15,1.3,1.48,1.72,2.05,1.72,4.02,
                  3.29,2.01,2.02,2.01,2.02,2.0,2.03,1.99,2.01,2.03,1.99,3.47];
  
etaRange = xrange(1,41) # 40 towers excluding ieta=29
nTT4Range = xrange(0,193)
addr = 0
printBins = ""

for nTT4 in nTT4Range:
    for ieta in etaRange:
        towEtThresh = int(math.floor((towerEtaWidths[ieta-1]/4.02)*ieta/5*(nTT4/16)))
        if towEtThresh > 20:
            towEtThresh = int(20)
        if (addr % 40) == 0:
            printBins = "    # nTT4 = " + str(3*nTT4) + " ieta = " + str(ieta)  
        else:
            printBins = ""
        towEtThreshLUTFile.write(
            str(addr) + " " + 
            str(towEtThresh) +
            printBins +
            "\n"
            )
        addr+=1

if addr < 8192:
    for addr in xrange(addr,8192):
        towEtThreshLUTFile.write(str(addr) + " " + str(0) + " # dummy\n")
        addr+=1

print "Done. Closing file..."

towEtThreshLUTFile.close()
