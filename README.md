# SRIM to ROOT Converter and Analysis Tool

## 1. Directory Contents

### Input data files (SRIM/TRIM)

- `target_Cu_11um` — Scenario 1 (11 µm Cu target)
- `backing_C_12um_target_Cu_11um.txt` — Scenario 2 (12 µm C backing + 11 µm Cu target)
- `backing_C_12um_target_Cu_22um.txt` — Scenario 3 (12 µm C backing + 22 µm Cu target)

### Source code (ROOT macros)

- `converter.C` — Reads the input text files and creates ROOT `TTree` objects.
- `analysis.C` — Performs the analysis, Gaussian fitting, plotting, and output of numerical results.

## 2. Running the Project

### Step 1: Convert the input data

Run the following commands in ROOT:

```text
.L converter.C
converter()
```

This creates the `.root` files containing the data trees.

### Step 2: Run the analysis

```text
.x analysis.C
```

The program displays two canvases:

- **Histograms** — Six histograms of energy and Y-position, two for each scenario.
- **Summary** — Plots showing the trends in the mean energy, energy straggling, and Y-position dispersion across the scenarios.

For each scenario, the following quantities are printed to the terminal:

- Mean energy
- Energy loss
- Energy dispersion
- Y-position dispersion

## 3. Physical Conclusions

The analysis of a 40K beam with an initial energy of 15.2 GeV passing through the target and backing system showed:

### A. Mean Kinetic Energy

- It decreases monotonically as the total material thickness increases (S1 → S2 → S3).
- The total energy loss is very small (< 0.2%), which is attributed to the high initial energy of the ions.

### B. Energy Dispersion (Straggling)

- It increases for the successive scenarios. Adding the carbon backing noticeably increases the energy broadening compared with the target alone.
- The energy distributions can be adequately described by Gaussian functions.

### C. Y-Position Dispersion

- It increases with the thickness of the material due to multiple Coulomb scattering.
- The beam becomes spatially broader, but the effect is on the order of micrometers.
