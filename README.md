# PLODE
Precise Logic and Delay (PLODE) is a tool that converts a Structural Verilog to a Spice deck, executes Spice simulations using ngspice and performs analysis of the Spice results to extract output delays and logic values. It can handle hierarchical Verilog with nested module instantiations. It takes into consideration glitches at the outputs in computing the output logic values and delays.</br>

To download and run the program correctly:
1. Download ngspice from its website and do its installation.
2. Download QT Creator with QT5 and QTCharts support.
3. Once all downloaded, it can be opened on QT Creator by opening the .pro file with QT Creator.
4. After building, PLODE GUI is ready to go.


# Build without QTCreator

For release:

```
qmake-qt5 PLODE.pro
make
```

For debug:

```
qmake-qt5 CONFIG+=debug PLODE.pro
make
```

# Some test cases

```txt
dcirc1 N30 stuck-at-0
PIs: N5[1] N4[x] N3[x] N2[0] N1[1] N0[x]
POs: N15[D] N14[x]

dcirc1 N30 stuck-at-1
PIs: N5[1] N4[x] N3[x] N2[1] N1[1] N0[0]
POs: N15[E] N14[x]

dcirc2 N25 stuck-at-0
PIs: N2[1] N1[1] N0[1]
POs: N7[E]

dcirc2 N25 stuck-at-1
PIs: N2[1] N1[1] N0[0]
POs: N7[D]

c17 N11 stuck-at-0
PIs: N7[1] N2[1] N6[x] N3[0] N1[x]
POs: N23[D] N22[x]

c17 N11 stuck-at-1
PIs: N7[1] N2[1] N6[1] N3[1] N1[x]
POs: N23[E] N22[x]

c17 sensitize path: N3 NAND2_2 N11 NAND2_4 N19 NAND2_6 N23
PIs: N7[1] N2[0] N6[1] N3[C] N1[x]
POs: N23[C] N22[x]

dcirc2 sensitize path: N0 AND2_4 N25 NOR2_6 N7
PIs: N2[1] N1[1] N0[C]
POs: N7[C]

provided 3 delay files for test circuits
```

# Test Circuits

d_test_circ_1.v

![dcirc1](Resources/dcirc1.png)

d_test_circ_2.v

![dcirc2](Resources/dcirc2.png)

c17.v

![c17circ](Resources/c17circ.png)

false_path.v

![falsepathcirc](Resources/falsepathcirc.png)
