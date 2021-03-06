.SUBCKT nand5 A1 A2 A3 A4 A5 VDD VSS ZN
**************************************NAND3_X2*************************************
M_M3 N_Zalp1_M0_d N_A3_M0_g N_VDD_M0_s VDD PMOS_VTL W=0.270000U L=0.050000U AS=0.028350P AD=0.037800P PS=0.750000U PD=0.820000U
M_M4 N_VDD_M1_d N_A2_M1_g N_Zalp1_M0_d VDD PMOS_VTL W=0.270000U L=0.050000U AS=0.037800P AD=0.037800P PS=0.820000U PD=0.820000U
M_M5 N_Zalp1_M2_d N_A1_M2_g N_VDD_M1_d VDD PMOS_VTL W=0.270000U L=0.050000U AS=0.037800P AD=0.028350P PS=0.820000U PD=0.750000U
M_M0 7 N_A3_M3_g N_VSS_M3_s VSS NMOS_VTL W=0.340000U L=0.050000U AS=0.035700P AD=0.047600P PS=0.890000U PD=0.960000U
M_M1 8 N_A2_M4_g 7 VSS NMOS_VTL W=0.340000U L=0.050000U AS=0.047600P AD=0.047600P PS=0.960000U PD=0.960000U
M_M2 N_Zalp1_M5_d N_A1_M5_g 8 VSS NMOS_VTL W=0.340000U L=0.050000U AS=0.047600P AD=0.035700P PS=0.960000U PD=0.890000U
C_x_PM_NAND3_X2__VSS_c0 VSS VSS 7.02872e-17
C_x_PM_NAND3_X2__VSS_c1 VSS x_PM_NAND3_X2__VSS_6 1.05487e-17
C_x_PM_NAND3_X2__VSS_c2 VSS N_VSS_M3_s 1.8134e-17
R_x_PM_NAND3_X2__VSS_r3 VSS x_PM_NAND3_X2__VSS_6 0.603529
R_x_PM_NAND3_X2__VSS_r4 x_PM_NAND3_X2__VSS_6 x_PM_NAND3_X2__VSS_2 0.264221
R_x_PM_NAND3_X2__VSS_r5 N_VSS_M3_s x_PM_NAND3_X2__VSS_2 0.230714
C_x_PM_NAND3_X2__VDD_c0 VSS N_VDD_M1_d 6.12848e-17
C_x_PM_NAND3_X2__VDD_c1 VSS x_PM_NAND3_X2__VDD_7 3.00804e-17
C_x_PM_NAND3_X2__VDD_c2 VSS N_VDD_M0_s 2.5125e-17
C_x_PM_NAND3_X2__VDD_c3 VSS x_PM_NAND3_X2__VDD_3 1.06055e-17
R_x_PM_NAND3_X2__VDD_r4 VDD x_PM_NAND3_X2__VDD_8 0.195294
R_x_PM_NAND3_X2__VDD_r5 x_PM_NAND3_X2__VDD_7 N_VDD_M1_d 0.140674
R_x_PM_NAND3_X2__VDD_r6 x_PM_NAND3_X2__VDD_8 x_PM_NAND3_X2__VDD_7 0.614706
R_x_PM_NAND3_X2__VDD_r7 VDD x_PM_NAND3_X2__VDD_3 0.0689273
R_x_PM_NAND3_X2__VDD_r8 N_VDD_M0_s x_PM_NAND3_X2__VDD_3 0.230714
C_x_PM_NAND3_X2__A3_c0 VSS x_PM_NAND3_X2__A3_14 6.68331e-18
C_x_PM_NAND3_X2__A3_c1 VSS x_PM_NAND3_X2__A3_12 3.18495e-17
C_x_PM_NAND3_X2__A3_c2 VSS N_A3_M0_g 7.81987e-17
C_x_PM_NAND3_X2__A3_c3 VSS N_A3_M3_g 4.14029e-17
R_x_PM_NAND3_X2__A3_r4 x_PM_NAND3_X2__A3_18 x_PM_NAND3_X2__A3_14 4.7687
R_x_PM_NAND3_X2__A3_r5 x_PM_NAND3_X2__A3_17 x_PM_NAND3_X2__A3_14 4.7687
R_x_PM_NAND3_X2__A3_r6 x_PM_NAND3_X2__A3_14 x_PM_NAND3_X2__A3_12 25.0012
R_x_PM_NAND3_X2__A3_r7 x_PM_NAND3_X2__A3_12 A3 0.0781486
R_x_PM_NAND3_X2__A3_r8 N_A3_M0_g x_PM_NAND3_X2__A3_18 95.94
R_x_PM_NAND3_X2__A3_r9 N_A3_M3_g x_PM_NAND3_X2__A3_17 35.1
C_x_PM_NAND3_X2__Zalp1_c0 VSS N_Zalp1_M5_d 1.32739e-16
C_x_PM_NAND3_X2__Zalp1_c1 VSS x_PM_NAND3_X2__Zalp1_8 1.01878e-17
C_x_PM_NAND3_X2__Zalp1_c2 VSS x_PM_NAND3_X2__Zalp1_4 7.15422e-17
R_x_PM_NAND3_X2__Zalp1_r3 Zalp1 N_Zalp1_M5_d 1.59389
R_x_PM_NAND3_X2__Zalp1_r4 N_Zalp1_M2_d x_PM_NAND3_X2__Zalp1_8 0.0406238
R_x_PM_NAND3_X2__Zalp1_r5 Zalp1 x_PM_NAND3_X2__Zalp1_8 1.28778
R_x_PM_NAND3_X2__Zalp1_r6 N_Zalp1_M2_d x_PM_NAND3_X2__Zalp1_4 0.176037
R_x_PM_NAND3_X2__Zalp1_r7 N_Zalp1_M0_d x_PM_NAND3_X2__Zalp1_4 1.85929
C_x_PM_NAND3_X2__A2_c0 VSS x_PM_NAND3_X2__A2_14 8.63262e-18
C_x_PM_NAND3_X2__A2_c1 VSS x_PM_NAND3_X2__A2_12 3.91329e-17
C_x_PM_NAND3_X2__A2_c2 VSS N_A2_M1_g 4.24405e-17
C_x_PM_NAND3_X2__A2_c3 VSS N_A2_M4_g 8.48173e-17
R_x_PM_NAND3_X2__A2_r4 x_PM_NAND3_X2__A2_18 x_PM_NAND3_X2__A2_14 4.7687
R_x_PM_NAND3_X2__A2_r5 x_PM_NAND3_X2__A2_17 x_PM_NAND3_X2__A2_14 4.7687
R_x_PM_NAND3_X2__A2_r6 x_PM_NAND3_X2__A2_14 x_PM_NAND3_X2__A2_12 25.0012
R_x_PM_NAND3_X2__A2_r7 x_PM_NAND3_X2__A2_12 A2 0.169643
R_x_PM_NAND3_X2__A2_r8 N_A2_M1_g x_PM_NAND3_X2__A2_18 29.64
R_x_PM_NAND3_X2__A2_r9 N_A2_M4_g x_PM_NAND3_X2__A2_17 101.4
C_x_PM_NAND3_X2__A1_c0 VSS x_PM_NAND3_X2__A1_18 1.22048e-17
C_x_PM_NAND3_X2__A1_c1 VSS x_PM_NAND3_X2__A1_12 7.61168e-17
C_x_PM_NAND3_X2__A1_c2 VSS N_A1_M2_g 8.98944e-17
C_x_PM_NAND3_X2__A1_c3 VSS N_A1_M5_g 4.86035e-17
R_x_PM_NAND3_X2__A1_r4 x_PM_NAND3_X2__A1_18 x_PM_NAND3_X2__A1_14 3.9
R_x_PM_NAND3_X2__A1_r5 x_PM_NAND3_X2__A1_14 x_PM_NAND3_X2__A1_12 25.0012
R_x_PM_NAND3_X2__A1_r6 x_PM_NAND3_X2__A1_12 A1 0.0459677
R_x_PM_NAND3_X2__A1_r7 x_PM_NAND3_X2__A1_18 x_PM_NAND3_X2__A1_5 1.95
R_x_PM_NAND3_X2__A1_r8 N_A1_M2_g x_PM_NAND3_X2__A1_5 95.94
R_x_PM_NAND3_X2__A1_r9 x_PM_NAND3_X2__A1_18 x_PM_NAND3_X2__A1_VSS 1.95
R_x_PM_NAND3_X2__A1_r10 N_A1_M5_g x_PM_NAND3_X2__A1_VSS 35.1

**************************************NAND2_X2**************************************
M2_M2 N2_Zalp2_M0_d N2_A5_M0_g N2_VDD_M0_s VDD PMOS_VTL W=0.270000U  AS=0.028350P AD=0.037800P PS=0.750000U PD=0.820000U
M2_M3 N2_VDD_M1_d N2_A4_M1_g N2_Zalp2_M0_d VDD PMOS_VTL W=0.270000U  AS=0.037800P AD=0.028350P PS=0.820000U PD=0.750000U
M2_M0 62 N2_A5_M2_g N2_VSS_M2_s VSS NMOS_VTL W=0.260000U  AS=0.027300P AD=0.036400P PS=0.730000U PD=0.800000U
M2_M1 N2_Zalp2_M3_d N2_A4_M3_g 62 VSS NMOS_VTL W=0.260000U  AS=0.036400P AD=0.027300P PS=0.800000U PD=0.730000U
C_x_PM_NAND2_X2__VSS_c0 VSS VSS 5.03826e-17
C_x_PM_NAND2_X2__VSS_c1 VSS x_PM_NAND2_X2__VSS_6 1.04465e-17
C_x_PM_NAND2_X2__VSS_c2 VSS N2_VSS_M2_s 1.83058e-17
R_x_PM_NAND2_X2__VSS_r3 VSS x_PM_NAND2_X2__VSS_6 0.391176
R_x_PM_NAND2_X2__VSS_r4 x_PM_NAND2_X2__VSS_6 x_PM_NAND2_X2__VSS_2 0.264221
R_x_PM_NAND2_X2__VSS_r5 N2_VSS_M2_s x_PM_NAND2_X2__VSS_2 0.230714
C_x_PM_NAND2_X2__VDD_c0 VSS N2_VDD_M1_d 4.75732e-17
C_x_PM_NAND2_X2__VDD_c1 VSS x_PM_NAND2_X2__VDD_7 4.67275e-17
C_x_PM_NAND2_X2__VDD_c2 VSS N2_VDD_M0_s 2.99432e-17
C_x_PM_NAND2_X2__VDD_c3 VSS x_PM_NAND2_X2__VDD_3 1.06496e-17
R_x_PM_NAND2_X2__VDD_r4 N2_VDD_M1_d x_PM_NAND2_X2__VDD_9 0.420714
R_x_PM_NAND2_X2__VDD_r5 VDD x_PM_NAND2_X2__VDD_8 0.195294
R_x_PM_NAND2_X2__VDD_r6 x_PM_NAND2_X2__VDD_9 x_PM_NAND2_X2__VDD_7 0.264221
R_x_PM_NAND2_X2__VDD_r7 x_PM_NAND2_X2__VDD_8 x_PM_NAND2_X2__VDD_7 0.681765
R_x_PM_NAND2_X2__VDD_r8 VDD x_PM_NAND2_X2__VDD_3 0.0689273
R_x_PM_NAND2_X2__VDD_r9 N2_VDD_M0_s x_PM_NAND2_X2__VDD_3 0.420714
C_x_PM_NAND2_X2__A5_c0 VSS x_PM_NAND2_X2__A5_14 7.51478e-18
C_x_PM_NAND2_X2__A5_c1 VSS A5 6.01437e-17
C_x_PM_NAND2_X2__A5_c2 VSS N2_A5_M0_g 8.26414e-17
C_x_PM_NAND2_X2__A5_c3 VSS N2_A5_M2_g 3.58741e-17
R_x_PM_NAND2_X2__A5_r4 x_PM_NAND2_X2__A5_18 x_PM_NAND2_X2__A5_14 4.74714
R_x_PM_NAND2_X2__A5_r5 x_PM_NAND2_X2__A5_17 x_PM_NAND2_X2__A5_14 4.74714
R_x_PM_NAND2_X2__A5_r6 x_PM_NAND2_X2__A5_14 x_PM_NAND2_X2__A5_12 25.0012
R_x_PM_NAND2_X2__A5_r7 x_PM_NAND2_X2__A5_12 A5 0.266
R_x_PM_NAND2_X2__A5_r8 N2_A5_M0_g x_PM_NAND2_X2__A5_18 103.74
R_x_PM_NAND2_X2__A5_r9 N2_A5_M2_g x_PM_NAND2_X2__A5_17 28.08
C_x_PM_NAND2_X2__Zalp2_c0 VSS N2_Zalp2_M3_d 9.46701e-17
C_x_PM_NAND2_X2__Zalp2_c1 VSS x_PM_NAND2_X2__Zalp2_8 7.68686e-18
C_x_PM_NAND2_X2__Zalp2_c2 VSS x_PM_NAND2_X2__Zalp2_7 3.24105e-17
C_x_PM_NAND2_X2__Zalp2_c3 VSS N2_Zalp2_M0_d 6.2878e-17
R_x_PM_NAND2_X2__Zalp2_r4 Zalp2 N2_Zalp2_M3_d 1.93167
R_x_PM_NAND2_X2__Zalp2_r5 Zalp2 x_PM_NAND2_X2__Zalp2_9 0.527778
R_x_PM_NAND2_X2__Zalp2_r6 x_PM_NAND2_X2__Zalp2_9 x_PM_NAND2_X2__Zalp2_7 0.21666
R_x_PM_NAND2_X2__Zalp2_r7 x_PM_NAND2_X2__Zalp2_8 x_PM_NAND2_X2__Zalp2_7 0.624286
R_x_PM_NAND2_X2__Zalp2_r8 x_PM_NAND2_X2__Zalp2_8 x_PM_NAND2_X2__Zalp2_3 0.212317
R_x_PM_NAND2_X2__Zalp2_r9 N2_Zalp2_M0_d x_PM_NAND2_X2__Zalp2_3 1.18071
C_x_PM_NAND2_X2__A4_c0 VSS x_PM_NAND2_X2__A4_18 1.23667e-17
C_x_PM_NAND2_X2__A4_c1 VSS A4 8.87208e-17
C_x_PM_NAND2_X2__A4_c2 VSS N2_A4_M1_g 9.96411e-17
C_x_PM_NAND2_X2__A4_c3 VSS N2_A4_M3_g 3.7962e-17
R_x_PM_NAND2_X2__A4_r4 x_PM_NAND2_X2__A4_18 x_PM_NAND2_X2__A4_14 3.9
R_x_PM_NAND2_X2__A4_r5 x_PM_NAND2_X2__A4_14 x_PM_NAND2_X2__A4_12 25.0012
R_x_PM_NAND2_X2__A4_r6 x_PM_NAND2_X2__A4_12 A4 0.316667
R_x_PM_NAND2_X2__A4_r7 x_PM_NAND2_X2__A4_18 x_PM_NAND2_X2__A4_5 1.95
R_x_PM_NAND2_X2__A4_r8 N2_A4_M1_g x_PM_NAND2_X2__A4_5 103.74
R_x_PM_NAND2_X2__A4_r9 x_PM_NAND2_X2__A4_18 x_PM_NAND2_X2__A4_VSS 1.95
R_x_PM_NAND2_X2__A4_r10 N2_A4_M3_g x_PM_NAND2_X2__A4_VSS 28.08

**************************************OR2_X2**************************************************
M3_M3 73 N3_Zalp1_M0_g N3_3_M0_s VDD PMOS_VTL W=0.195000U AS=0.020475P AD=0.027300P PS=0.600000U PD=0.670000U
M3_M4 N3_VDD_M1_d N3_Zalp2_M1_g 73 VDD PMOS_VTL W=0.195000U AS=0.027300P AD=0.037950P PS=0.670000U PD=0.860000U
M3_M5 N3_ZN3_M2_d N3_3_M2_g N3_VDD_M1_d VDD PMOS_VTL W=0.270000U AS=0.037950P AD=0.028350P PS=0.860000U PD=0.750000U
M3_M0 N3_3_M3_d N3_Zalp1_M3_g N3_VSS_M3_s VSS NMOS_VTL W=0.090000U AS=0.009450P AD=0.012600P PS=0.390000U PD=0.460000U
M3_M1 N3_VSS_M4_d N3_Zalp2_M4_g N3_3_M3_d VSS NMOS_VTL W=0.090000U AS=0.012600P AD=0.023850P PS=0.460000U PD=0.680000U
M3_M2 N3_ZN3_M5_d N3_3_M5_g N3_VSS_M4_d VSS NMOS_VTL W=0.180000U AS=0.023850P AD=0.018900P PS=0.680000U PD=0.570000U
C_x_PM_OR2_X2__VSS_c0 VSS x_PM_OR2_X2__VSS_15 3.72032e-17
C_x_PM_OR2_X2__VSS_c1 VSS N3_VSS_M4_d 1.48795e-17
C_x_PM_OR2_X2__VSS_c2 VSS x_PM_OR2_X2__VSS_6 1.07612e-17
C_x_PM_OR2_X2__VSS_c3 VSS x_PM_OR2_X2__VSS_5 3.45483e-17
C_x_PM_OR2_X2__VSS_c4 VSS x_PM_OR2_X2__VSS_4 2.26908e-17
R_x_PM_OR2_X2__VSS_r5 x_PM_OR2_X2__VSS_15 x_PM_OR2_X2__VSS_9 0.145286
R_x_PM_OR2_X2__VSS_r6 N3_VSS_M4_d x_PM_OR2_X2__VSS_9 0.230714
R_x_PM_OR2_X2__VSS_r7 VSS x_PM_OR2_X2__VSS_6 0.603529
R_x_PM_OR2_X2__VSS_r8 x_PM_OR2_X2__VSS_15 x_PM_OR2_X2__VSS_5 0.0731438
R_x_PM_OR2_X2__VSS_r9 VSS x_PM_OR2_X2__VSS_5 0.134118
R_x_PM_OR2_X2__VSS_r10 x_PM_OR2_X2__VSS_6 x_PM_OR2_X2__VSS_4 0.264221
R_x_PM_OR2_X2__VSS_r11 x_PM_OR2_X2__VSS_4 N3_VSS_M3_s 0.543196
C_x_PM_OR2_X2__VDD_c0 VSS x_PM_OR2_X2__VDD_12 4.05231e-17
C_x_PM_OR2_X2__VDD_c1 VSS N3_VDD_M1_d 2.06356e-17
C_x_PM_OR2_X2__VDD_c2 VSS x_PM_OR2_X2__VDD_2 4.07936e-17
R_x_PM_OR2_X2__VDD_r3 x_PM_OR2_X2__VDD_12 x_PM_OR2_X2__VDD_6 0.145286
R_x_PM_OR2_X2__VDD_r4 N3_VDD_M1_d x_PM_OR2_X2__VDD_6 0.420714
R_x_PM_OR2_X2__VDD_r5 x_PM_OR2_X2__VDD_12 x_PM_OR2_X2__VDD_2 0.0731438
R_x_PM_OR2_X2__VDD_r6 VDD x_PM_OR2_X2__VDD_2 0.0782353
C_x_PM_OR2_X2__3_c0 VSS x_PM_OR2_X2__3_31 1.75553e-17
C_x_PM_OR2_X2__3_c1 VSS x_PM_OR2_X2__3_28 1.72947e-17
C_x_PM_OR2_X2__3_c2 VSS x_PM_OR2_X2__3_25 5.24935e-17
C_x_PM_OR2_X2__3_c3 VSS x_PM_OR2_X2__3_22 2.8529e-17
C_x_PM_OR2_X2__3_c4 VSS N3_3_M3_d 4.52149e-17
C_x_PM_OR2_X2__3_c5 VSS x_PM_OR2_X2__3_16 1.69957e-17
C_x_PM_OR2_X2__3_c6 VSS x_PM_OR2_X2__3_15 3.68802e-17
C_x_PM_OR2_X2__3_c7 VSS N3_3_M0_s 4.3344e-17
C_x_PM_OR2_X2__3_c8 VSS N3_3_M2_g 9.38216e-17
C_x_PM_OR2_X2__3_c9 VSS N3_3_M5_g 4.44376e-17
R_x_PM_OR2_X2__3_r10 x_PM_OR2_X2__3_31 x_PM_OR2_X2__3_29 7.54
R_x_PM_OR2_X2__3_r11 x_PM_OR2_X2__3_29 x_PM_OR2_X2__3_25 25.0012
R_x_PM_OR2_X2__3_r12 x_PM_OR2_X2__3_28 x_PM_OR2_X2__3_23 0.0418175
R_x_PM_OR2_X2__3_r13 x_PM_OR2_X2__3_25 x_PM_OR2_X2__3_23 1.045
R_x_PM_OR2_X2__3_r14 x_PM_OR2_X2__3_28 x_PM_OR2_X2__3_21 0.160909
R_x_PM_OR2_X2__3_r15 x_PM_OR2_X2__3_22 x_PM_OR2_X2__3_21 0.705714
R_x_PM_OR2_X2__3_r16 x_PM_OR2_X2__3_28 x_PM_OR2_X2__3_17 0.160909
R_x_PM_OR2_X2__3_r17 N3_3_M3_d x_PM_OR2_X2__3_17 1.045
R_x_PM_OR2_X2__3_r18 x_PM_OR2_X2__3_22 x_PM_OR2_X2__3_15 0.212317
R_x_PM_OR2_X2__3_r19 x_PM_OR2_X2__3_16 x_PM_OR2_X2__3_15 0.651429
R_x_PM_OR2_X2__3_r20 x_PM_OR2_X2__3_16 x_PM_OR2_X2__3_11 0.212317
R_x_PM_OR2_X2__3_r21 N3_3_M0_s x_PM_OR2_X2__3_11 2.29357
R_x_PM_OR2_X2__3_r22 x_PM_OR2_X2__3_31 x_PM_OR2_X2__3_5 1.95
R_x_PM_OR2_X2__3_r23 N3_3_M2_g x_PM_OR2_X2__3_5 99.84
R_x_PM_OR2_X2__3_r24 x_PM_OR2_X2__3_31 x_PM_OR2_X2__3_VSS 1.95
R_x_PM_OR2_X2__3_r25 N3_3_M5_g x_PM_OR2_X2__3_VSS 38.22
C_x_PM_OR2_X2__Zalp1_c0 VSS x_PM_OR2_X2__Zalp1_18 8.81315e-18
C_x_PM_OR2_X2__Zalp1_c1 VSS x_PM_OR2_X2__Zalp1_12 5.4822e-17
C_x_PM_OR2_X2__Zalp1_c2 VSS N3_Zalp1_M0_g 8.90209e-17
C_x_PM_OR2_X2__Zalp1_c3 VSS N3_Zalp1_M3_g 3.57915e-17
R_x_PM_OR2_X2__Zalp1_r4 x_PM_OR2_X2__Zalp1_18 x_PM_OR2_X2__Zalp1_14 3.38
R_x_PM_OR2_X2__Zalp1_r5 x_PM_OR2_X2__Zalp1_14 x_PM_OR2_X2__Zalp1_12 25.0012
R_x_PM_OR2_X2__Zalp1_r6 x_PM_OR2_X2__Zalp1_12 Zalp1 0.115357
R_x_PM_OR2_X2__Zalp1_r7 x_PM_OR2_X2__Zalp1_18 x_PM_OR2_X2__Zalp1_5 1.95
R_x_PM_OR2_X2__Zalp1_r8 N3_Zalp1_M0_g x_PM_OR2_X2__Zalp1_5 101.01
R_x_PM_OR2_X2__Zalp1_r9 x_PM_OR2_X2__Zalp1_18 x_PM_OR2_X2__Zalp1_VSS 1.95
R_x_PM_OR2_X2__Zalp1_r10 N3_Zalp1_M3_g x_PM_OR2_X2__Zalp1_VSS 35.88
C_x_PM_OR2_X2__Zalp2_c0 VSS Zalp2 6.95827e-17
C_x_PM_OR2_X2__Zalp2_c1 VSS x_PM_OR2_X2__Zalp2_11 1.12977e-17
C_x_PM_OR2_X2__Zalp2_c2 VSS N3_Zalp2_M1_g 6.44459e-17
C_x_PM_OR2_X2__Zalp2_c3 VSS N3_Zalp2_M4_g 5.20861e-17
R_x_PM_OR2_X2__Zalp2_r4 x_PM_OR2_X2__Zalp2_11 x_PM_OR2_X2__Zalp2_16 4.42
R_x_PM_OR2_X2__Zalp2_r5 x_PM_OR2_X2__Zalp2_11 x_PM_OR2_X2__Zalp2_9 25.0012
R_x_PM_OR2_X2__Zalp2_r6 Zalp2 x_PM_OR2_X2__Zalp2_9 0.196786
R_x_PM_OR2_X2__Zalp2_r7 x_PM_OR2_X2__Zalp2_16 x_PM_OR2_X2__Zalp2_5 1.95
R_x_PM_OR2_X2__Zalp2_r8 N3_Zalp2_M1_g x_PM_OR2_X2__Zalp2_5 79.95
R_x_PM_OR2_X2__Zalp2_r9 x_PM_OR2_X2__Zalp2_16 x_PM_OR2_X2__Zalp2_VSS 1.95
R_x_PM_OR2_X2__Zalp2_r10 N3_Zalp2_M4_g x_PM_OR2_X2__Zalp2_VSS 56.94
C_x_PM_OR2_X2__ZN3_c0 VSS N3_ZN3_M5_d 1.3351e-16
R_x_PM_OR2_X2__ZN3_r1 N3_ZN3_M2_d ZN 2.23929
R_x_PM_OR2_X2__ZN3_r2 ZN N3_ZN3_M5_d 2.91786
.ENDS 

********************************************************************************
*
* END
*
********************************************************************************
