// -*- coding: windows-1252 -*-
#include "TestIPhreeqc.h"

#include <cmath>
#include <cfloat>
#include <cassert>
#include "IPhreeqc.hpp"
#include "Phreeqc.h"
#include "FileTest.h"
#undef true
#undef false
#include "CVar.hxx"

VRESULT SOLUTION(IPhreeqc& obj, double C, double Ca, double Na);
VRESULT EQUILIBRIUM_PHASES(IPhreeqc& obj, const char* phase, double si, double amount);
VRESULT USER_PUNCH(IPhreeqc& obj, const char* element, int max);
VRESULT SELECTED_OUTPUT(IPhreeqc& obj);
VRESULT DUMP(IPhreeqc& obj);

TestIPhreeqc::TestIPhreeqc(void)
{
}

TestIPhreeqc::~TestIPhreeqc(void)
{
}

void TestIPhreeqc::TestLoadDatabase(void)
{
	std::string FILES[] = {"../database/phreeqc.dat", "../database/pitzer.dat", "../database/wateq4f.dat",
		"../database/Amm.dat", "../database/frezchem.dat", "../database/iso.dat",
		"../database/llnl.dat", "../database/minteq.dat", "../database/minteq.v4.dat",
		"../database/sit.dat","../database/ColdChem.dat","../database/core10.dat",
		"../database/Tipping_Hurley.dat"
	};
	for (int j = 0; j < sizeof(FILES)/sizeof(std::string); ++j)
	{
		IPhreeqc obj;
		for (int i = 0; i < 10; ++i)
		{
			CPPUNIT_ASSERT_EQUAL(true, ::FileExists(FILES[j].c_str()));
			CPPUNIT_ASSERT(::FileSize(FILES[j].c_str()) > 0);
			CPPUNIT_ASSERT_EQUAL(0,    obj.LoadDatabase(FILES[j].c_str()));
		}

		// make sure settings are cleared
		//

		IPhreeqc obj2;
		for (int i = 0; i < 10; ++i)
		{
			CPPUNIT_ASSERT_EQUAL(false, obj2.GetSelectedOutputFileOn());

			obj2.SetSelectedOutputFileOn(true);
			CPPUNIT_ASSERT_EQUAL(true, obj2.GetSelectedOutputFileOn());

			obj2.SetSelectedOutputFileOn(true);
			CPPUNIT_ASSERT_EQUAL(true, obj2.GetSelectedOutputFileOn());

			CPPUNIT_ASSERT_EQUAL(true, ::FileExists(FILES[j].c_str()));
			CPPUNIT_ASSERT(::FileSize(FILES[j].c_str()) > 0);
			CPPUNIT_ASSERT_EQUAL(0,    obj2.LoadDatabase(FILES[j].c_str()));

			// all previous definitions are cleared
			CPPUNIT_ASSERT_EQUAL(false, obj2.GetSelectedOutputFileOn());
		}
	}
}

void TestIPhreeqc::TestLoadDatabaseString(void)
{
	const char ex15_dat[] =
		"SOLUTION_MASTER_SPECIES\n"
		"C        CO2            2.0     61.0173         12.0111\n"
		"Cl       Cl-            0.0     Cl              35.453\n"
		"Co       Co+2           0.0     58.93           58.93   \n"
		"E        e-             0.0     0.0             0.0\n"
		"H        H+             -1.     1.008           1.008\n"
		"H(0)     H2             0.0     1.008\n"
		"H(1)     H+             -1.     1.008\n"
		"N        NH4+           0.0     14.0067         14.0067\n"
		"Na       Na+            0.0     Na              22.9898\n"
		"Nta      Nta-3          3.0     1.              1.\n"
		"O        H2O            0.0     16.00           16.00\n"
		"O(-2)    H2O            0.0     18.016\n"
		"O(0)     O2             0.0     16.00\n"
		"SOLUTION_SPECIES\n"
		"2H2O = O2 + 4H+ + 4e- \n"
		"        log_k   -86.08; -gamma  1e7   0.0\n"
		"2 H+ + 2 e- = H2\n"
		"        log_k   -3.15;  -gamma  1e7   0.0\n"
		"H+ = H+\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"e- = e-\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"H2O = H2O\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"CO2 = CO2\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"Na+ = Na+\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"Cl- = Cl-\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"Co+2 = Co+2\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"NH4+ = NH4+\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"Nta-3 = Nta-3\n"
		"        log_k   0.0;    -gamma  1e7   0.0\n"
		"Nta-3 + 3H+ = H3Nta\n"
		"        log_k   14.9;   -gamma  1e7   0.0\n"
		"Nta-3 + 2H+ = H2Nta-\n"
		"        log_k   13.3;   -gamma  1e7   0.0\n"
		"Nta-3 + H+ = HNta-2\n"
		"        log_k   10.3;   -gamma  1e7   0.0\n"
		"Nta-3 + Co+2 = CoNta-\n"
		"        log_k   11.7;   -gamma  1e7   0.0\n"
		"2 Nta-3 + Co+2 = CoNta2-4\n"
		"        log_k   14.5;   -gamma  1e7   0.0\n"
		"Nta-3 + Co+2 + H2O = CoOHNta-2 + H+\n"
		"        log_k   0.5;    -gamma  1e7   0.0\n"
		"Co+2 + H2O = CoOH+ + H+\n"
		"        log_k   -9.7;   -gamma  1e7   0.0\n"
		"Co+2 + 2H2O = Co(OH)2 + 2H+\n"
		"        log_k   -22.9;  -gamma  1e7   0.0\n"
		"Co+2 + 3H2O = Co(OH)3- + 3H+\n"
		"        log_k   -31.5;  -gamma  1e7   0.0\n"
		"CO2 + H2O = HCO3- + H+\n"
		"        log_k   -6.35;  -gamma  1e7   0.0\n"
		"CO2 + H2O = CO3-2 + 2H+\n"
		"        log_k   -16.68; -gamma  1e7   0.0\n"
		"NH4+ = NH3 + H+\n"
		"        log_k   -9.3;   -gamma  1e7   0.0\n"
		"H2O = OH- +  H+\n"
		"        log_k   -14.0;  -gamma  1e7   0.0\n"
		"END\n";

	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabaseString(ex15_dat));
}

void TestIPhreeqc::TestLoadDatabaseStringBadInput(void)
{
	IPhreeqc obj;

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT(0 != obj.LoadDatabaseString("phreeqc.dat.list"));
	}
}

void TestIPhreeqc::TestLoadDatabaseEx14AsDB(void)
{
	const char ex14[] =
		"TITLE Example 14.--Transport with equilibrium_phases, exchange, and surface reactions\n"
		"#\n"
		"# Use phreeqc.dat\n"
		"# Dzombak and Morel (1990) aqueous and surface complexation models for arsenic\n"
		"# are defined here\n"
		"#\n"
		"SURFACE_MASTER_SPECIES\n"
		"        Surf    SurfOH\n"
		"SURFACE_SPECIES\n"
		"        SurfOH = SurfOH\n"
		"                log_k   0.0\n"
		"        SurfOH  + H+ = SurfOH2+\n"
		"                log_k   7.29\n"
		"        SurfOH = SurfO- + H+\n"
		"                log_k   -8.93\n"
		"        SurfOH + AsO4-3 + 3H+ = SurfH2AsO4 + H2O\n"
		"                log_k   29.31\n"
		"        SurfOH + AsO4-3 + 2H+ = SurfHAsO4- + H2O\n"
		"                log_k   23.51\n"
		"        SurfOH + AsO4-3 = SurfOHAsO4-3\n"
		"                log_k   10.58\n"
		"SOLUTION_MASTER_SPECIES\n"
		"        As       H3AsO4        -1.0     74.9216    74.9216\n"
		"SOLUTION_SPECIES\n"
		"        H3AsO4 = H3AsO4\n"
		"                log_k           0.0\n"
		"        H3AsO4 = AsO4-3 + 3H+\n"
		"                log_k   -20.7\n"
		"        H+ + AsO4-3 = HAsO4-2\n"
		"                log_k   11.50\n"
		"        2H+ + AsO4-3 = H2AsO4-\n"
		"                log_k           18.46\n"
		"SOLUTION 1 Brine\n"
		"        pH      5.713\n"
		"        pe      4.0     O2(g)   -0.7\n"
		"        temp    25.\n"
		"        units   mol/kgw\n"
		"        Ca      .4655\n"
		"        Mg      .1609\n"
		"        Na      5.402\n"
		"        Cl      6.642           charge\n"
		"        C       .00396\n"
		"        S       .004725\n"
		"        As      .025 umol/kgw\n"
		"END\n"
		"USE solution 1\n"
		"EQUILIBRIUM_PHASES 1\n"
		"        Dolomite        0.0     1.6\n"
		"        Calcite         0.0     0.1\n"
		"SAVE solution 1\n"
		"# prints initial condition to the selected-output file\n"
		"SELECTED_OUTPUT\n"
		"        -file ex14.sel\n"
		"        -reset false\n"
		"        -step\n"
		"USER_PUNCH\n"
		"        -head  m_Ca m_Mg m_Na umol_As pH mmol_sorbedAs\n"
		"  10 PUNCH TOT(\"Ca\"), TOT(\"Mg\"), TOT(\"Na\"), TOT(\"As\")*1e6, -LA(\"H+\"), SURF(\"As\", \"Surf\")*1000\n"
		"END\n"
		"PRINT\n"
		"# skips print of initial exchange and initial surface to the selected-output file\n"
		"        -selected_out false\n"
		"EXCHANGE 1\n"
		"        -equil with solution 1\n"
		"        X       1.0\n"
		"SURFACE 1\n"
		"        -equil solution 1\n"
		"# assumes 1/10 of iron is HFO\n"
		"        SurfOH           0.07    600.    30.\n"
		"END\n"
		"SOLUTION 0 20 x precipitation\n"
		"        pH      4.6\n"
		"        pe      4.0     O2(g)   -0.7\n"
		"        temp    25.\n"
		"        units   mmol/kgw\n"
		"        Ca      .191625\n"
		"        Mg      .035797\n"
		"        Na      .122668\n"
		"        Cl      .133704\n"
		"        C       .01096\n"
		"        S       .235153         charge\n"
		"EQUILIBRIUM_PHASES 0\n"
		"        Dolomite        0.0     1.6\n"
		"        Calcite         0.0     0.1\n"
		"        CO2(g)          -1.5    10.\n"
		"SAVE solution 0\n"
		"END\n"
		"PRINT\n"
		"        -selected_out true\n"
		"        -status false\n"
		"ADVECTION\n"
		"        -cells 1\n"
		"        -shifts 200\n"
		"        -print_frequency 200\n"
		"USER_GRAPH 1 Example 14\n"
		"        -headings PV As(ppb) Ca(M) Mg(M) Na(M) pH\n"
		"        -chart_title \"Chemical Evolution of the Central Oklahoma Aquifer\"\n"
		"        -axis_titles \"Pore volumes or shift number\" \"Log(Concentration, in ppb or molal)\" \"pH\"\n"
		"        -axis_scale x_axis 0 200\n"
		"        -axis_scale y_axis 1e-6 100 auto auto Log\n"
		"  10 GRAPH_X STEP_NO\n"
		"  20 GRAPH_Y TOT(\"As\") * 74.92e6, TOT(\"Ca\"), TOT(\"Mg\"), TOT(\"Na\")\n"
		"  30 GRAPH_SY -LA(\"H+\")\n"
		"END\n";

	IPhreeqc obj;
	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT(0 != obj.LoadDatabaseString(ex14));
	}
}

void TestIPhreeqc::TestLoadDatabaseMissingFile(void)
{
	CPPUNIT_ASSERT_EQUAL(false, ::FileExists("missing.file"));

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(1,     obj.LoadDatabase("missing.file"));
	CPPUNIT_ASSERT_EQUAL(1,     obj.LoadDatabase("missing.file"));
	CPPUNIT_ASSERT_EQUAL(1,     obj.LoadDatabase("missing.file"));

	const char expected[] =
		"ERROR: LoadDatabase: Unable to open:\"missing.file\".\n";

	const char* err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );
}

void TestIPhreeqc::TestLoadDatabaseWithErrors(void)
{
#if defined(_WIN32)
	int n0 = ::_fcloseall();
	assert(n0 == 0);
#endif

	IPhreeqc obj;

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(true, ::FileExists("missing_e.dat"));
		CPPUNIT_ASSERT(::FileSize("missing_e.dat") > 0);
		CPPUNIT_ASSERT_EQUAL(6,    obj.LoadDatabase("missing_e.dat"));

		const char *expected =
			"ERROR: Could not reduce equation to primary master species, CH4.\n"
			"ERROR: Could not reduce equation to primary master species, Cu+.\n"
			"ERROR: Could not reduce equation to primary master species, Fe+3.\n"
			"ERROR: Could not reduce equation to primary master species, H2.\n"
			"ERROR: Could not reduce equation to primary master species, Mn+3.\n"
			"ERROR: Could not reduce equation to primary master species, NH4+.\n"
			"ERROR: Could not reduce equation to primary master species, N2.\n"
			"ERROR: Could not reduce equation to primary master species, NO2-.\n"
			"ERROR: Could not reduce equation to primary master species, O2.\n"
			"ERROR: Could not reduce equation to primary master species, HS-.\n"
			"ERROR: Could not reduce equation to secondary master species, e-.\n"
			"ERROR: Non-master species in secondary reaction, e-.\n"
			"ERROR: No master species for element e.\n"
			"ERROR: Could not find primary master species for e.\n"
			"ERROR: No master species for element e.\n"
			"ERROR: Could not reduce equation to secondary master species, Hausmannite.\n"
			"ERROR: Could not reduce equation to secondary master species, Manganite.\n"
			"ERROR: Could not reduce equation to secondary master species, Pyrite.\n"
			"ERROR: Could not reduce equation to secondary master species, Pyrolusite.\n"
			"ERROR: Could not reduce equation to secondary master species, Sulfur.\n"
			"ERROR: e-, primary master species for E-, not defined.\n"
			"ERROR: Calculations terminating due to input errors.\n";

		const char* err = obj.GetErrorString();
		CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );
	}
#if defined(_WIN32)
	int n = ::_fcloseall();
	assert(n == 0);
#endif
}

void TestIPhreeqc::TestRunAccumulated(void)
{
#if defined(_WIN32)
	int n = ::_fcloseall();
	assert(n == 0);
#endif

	bool files_on = false;
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 12"));
	obj.SetOutputFileOn(files_on);
	obj.SetErrorFileOn(files_on);
	obj.SetLogFileOn(files_on);
	obj.SetSelectedOutputFileOn(files_on);
	obj.SetDumpFileOn(files_on);
	CPPUNIT_ASSERT_EQUAL(0,     obj.RunAccumulated());
}

void TestIPhreeqc::TestRunAccumulatedWithDBKeyword(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("DATABASE wateq4f.dat"));
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("SOLUTION 1"));
		CPPUNIT_ASSERT_EQUAL(0,     obj.RunAccumulated());

		const char* warn = obj.GetWarningString();
		const char expected[] = "WARNING: DATABASE keyword is ignored by IPhreeqc.\n";

		CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(warn));
	}
}

void TestIPhreeqc::TestDatabaseNotFirstKeyword(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("SOLUTION 1"));
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("DATABASE wateq4f.dat"));
		CPPUNIT_ASSERT_EQUAL(2,     obj.RunAccumulated());

		const char expected[] =
			"ERROR: DATABASE must be the first keyword in the input file.\n"
			"ERROR: Calculations terminating due to input errors.\n";
		const char* err = obj.GetErrorString();

		CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );
	}
}

void TestIPhreeqc::TestRunWithErrors(void)
{
	const char dump_file[] = "error.inp";
	IPhreeqc obj;

	FileTest dfile(dump_file);
	CPPUNIT_ASSERT( dfile.RemoveExisting() );

	bool files_on = false;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("SOLUTION 1"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	pH	7"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	Na	1"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("PHASES"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	Fix_H+"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	H+ = H+"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	log_k	0"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("END"));

	obj.SetOutputFileOn(files_on);
	obj.SetErrorFileOn(files_on);
	obj.SetLogFileOn(files_on);
	obj.SetSelectedOutputFileOn(files_on);
	obj.SetDumpFileOn(files_on);
	CPPUNIT_ASSERT_EQUAL(1,     obj.RunAccumulated());

	const char expected[] =
		"ERROR: Numerical method failed on all combinations of convergence parameters, cell/soln/mix 1\n";
	const char* err = obj.GetErrorString();

	CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );

	CPPUNIT_ASSERT( dfile.VerifyExists() );
	CPPUNIT_ASSERT( dfile.Size() > 0 );
}

void TestIPhreeqc::TestRunFile(void)
{
	const char dump_file[] = "error.inp";

	FileTest dfile(dump_file);
	CPPUNIT_ASSERT( dfile.RemoveExisting() );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL(1, obj.RunFile("conv_fail.in"));

	const char expected[] =
		"ERROR: Numerical method failed on all combinations of convergence parameters, cell/soln/mix 1\n";
	const char* err = obj.GetErrorString();

	CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(err));

	// Note: should this file exist since GetDumpFileOn is false?
	CPPUNIT_ASSERT( dfile.VerifyExists() );
	CPPUNIT_ASSERT( dfile.Size() > 0 );
}

void TestIPhreeqc::TestRunString(void)
{
	const char input[] =
		"TITLE Example 1.--Add uranium and speciate seawater.\n"
		"SOLUTION 1  SEAWATER FROM NORDSTROM ET AL. (1979)\n"
		"        units   ppm\n"
		"        pH      8.22\n"
		"        pe      8.451\n"
		"        density 1.023\n"
		"        temp    25.0\n"
		"        redox   O(0)/O(-2)\n"
		"        Ca              412.3\n"
		"        Mg              1291.8\n"
		"        Na              10768.0\n"
		"        K               399.1\n"
		"        Fe              0.002\n"
		"        Mn              0.0002  pe\n"
		"        Si              4.28\n"
		"        Cl              19353.0\n"
		"        Alkalinity      141.682 as HCO3\n"
		"        S(6)            2712.0\n"
		"        N(5)            0.29    gfw   62.0\n"
		"        N(-3)           0.03    as    NH4\n"
		"        U               3.3     ppb   N(5)/N(-3)\n"
		"        O(0)            1.0     O2(g) -0.7\n"
		"SOLUTION_MASTER_SPECIES\n"
		"        U       U+4     0.0     238.0290     238.0290\n"
		"        U(4)    U+4     0.0     238.0290\n"
		"        U(5)    UO2+    0.0     238.0290\n"
		"        U(6)    UO2+2   0.0     238.0290\n"
		"SOLUTION_SPECIES\n"
		"        #primary master species for U\n"
		"        #is also secondary master species for U(4)\n"
		"        U+4 = U+4\n"
		"                log_k          0.0\n"
		"        U+4 + 4 H2O = U(OH)4 + 4 H+\n"
		"                log_k          -8.538\n"
		"                delta_h        24.760 kcal\n"
		"        U+4 + 5 H2O = U(OH)5- + 5 H+\n"
		"                log_k          -13.147\n"
		"                delta_h        27.580 kcal\n"
		"        #secondary master species for U(5)\n"
		"        U+4 + 2 H2O = UO2+ + 4 H+ + e-\n"
		"                log_k          -6.432\n"
		"                delta_h        31.130 kcal\n"
		"        #secondary master species for U(6)\n"
		"        U+4 + 2 H2O = UO2+2 + 4 H+ + 2 e-\n"
		"                log_k          -9.217\n"
		"                delta_h        34.430 kcal\n"
		"        UO2+2 + H2O = UO2OH+ + H+\n"
		"                log_k          -5.782\n"
		"                delta_h        11.015 kcal\n"
		"        2UO2+2 + 2H2O = (UO2)2(OH)2+2 + 2H+\n"
		"                log_k          -5.626\n"
		"                delta_h        -36.04 kcal\n"
		"        3UO2+2 + 5H2O = (UO2)3(OH)5+ + 5H+\n"
		"                log_k          -15.641\n"
		"                delta_h        -44.27 kcal\n"
		"        UO2+2 + CO3-2 = UO2CO3\n"
		"                log_k          10.064\n"
		"                delta_h        0.84 kcal\n"
		"        UO2+2 + 2CO3-2 = UO2(CO3)2-2\n"
		"                log_k          16.977\n"
		"                delta_h        3.48 kcal\n"
		"        UO2+2 + 3CO3-2 = UO2(CO3)3-4\n"
		"                log_k          21.397\n"
		"                delta_h        -8.78 kcal\n"
		"PHASES\n"
		"        Uraninite\n"
		"        UO2 + 4 H+ = U+4 + 2 H2O\n"
		"        log_k          -3.490\n"
		"        delta_h        -18.630 kcal\n"
		"END\n"
		"\n";

	IPhreeqc obj;

	char OUTPUT_FILE[80];
	sprintf(OUTPUT_FILE, "phreeqc.%lu.out", (unsigned long)obj.Index);
	
	FileTest ofile(OUTPUT_FILE);
	CPPUNIT_ASSERT( ofile.RemoveExisting() );

	CPPUNIT_ASSERT_EQUAL(false, ::FileExists(OUTPUT_FILE));
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	obj.SetOutputFileOn(1);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	CPPUNIT_ASSERT_EQUAL(false, ::FileExists(OUTPUT_FILE));
	CPPUNIT_ASSERT_EQUAL(0,     obj.RunString(input));

	CPPUNIT_ASSERT( ofile.VerifyExists() );
	CPPUNIT_ASSERT( ofile.Size() > 0 );
}

void TestIPhreeqc::TestGetSelectedOutputRowCount(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("llnl.dat"));

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(VR_OK, EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010));
	CPPUNIT_ASSERT_EQUAL(VR_OK, USER_PUNCH(obj, "Ca", max));

	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunAccumulated());

	CPPUNIT_ASSERT_EQUAL(3, obj.GetSelectedOutputRowCount()); // rows + header
}

void TestIPhreeqc::TestGetSelectedOutputValue(void)
{
	int col;

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("llnl.dat"));

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(VR_OK, EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010));
	CPPUNIT_ASSERT_EQUAL(VR_OK, USER_PUNCH(obj, "Ca", max));

	obj.SetOutputFileOn(0);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunAccumulated());

/*
EXPECTED selected.out:
         sim	       state	        soln	      dist_x	        time	        step	          pH	          pe	           C	          Ca	          Na	     m_CO3-2	     m_CaOH+	    m_NaCO3-	    la_CO3-2	    la_CaOH+	   la_NaCO3-	     Calcite	   d_Calcite	   si_CO2(g)	 si_Siderite	    pressure	   total mol	      volume	    g_CO2(g)	     g_N2(g)	    k_Albite	   dk_Albite	    k_Pyrite	   dk_Pyrite	     s_CaSO4	     s_SrSO4	      1.name	      1.type	     1.moles	      2.name	      2.type	     2.moles	      3.name	      3.type	     3.moles	      4.name	      4.type	     4.moles	      5.name	      5.type	     5.moles	      6.name	      6.type	     6.moles
           1	      i_soln	           1	         -99	         -99	         -99	           7	           4	 1.0000e-003	 1.0000e-003	 1.0000e-003	 4.2975e-007	 1.1819e-009	 1.1881e-009	-6.4686e+000	-8.9530e+000	-8.9507e+000	 0.0000e+000	 0.0000e+000	     -2.2870	   -999.9990	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	        Ca+2	          aq	 9.9178e-004	     CaHCO3+	          aq	 7.5980e-006	       CaCO3	          aq	 6.2155e-007	       CaOH+	          aq	 1.1819e-009
           1	       react	           1	         -99	           0	           1	     7.86135	       10.18	 1.1556e-003	 1.1556e-003	 1.0000e-003	 4.2718e-006	 9.7385e-009	 1.1620e-008	-5.4781e+000	-8.0388e+000	-7.9621e+000	 9.8444e-003	-1.5555e-004	     -3.0192	   -999.9990	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	     calcite	        equi	 9.8444e-003	        Ca+2	          aq	 1.1371e-003	     CaHCO3+	          aq	 1.1598e-005	       CaCO3	          aq	 6.8668e-006	       CaOH+	          aq	 9.7385e-009
*/


	CVar v;

	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue(-1, 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_ERROR, v.type);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, v.vresult);

	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue(obj.GetSelectedOutputRowCount(), 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_ERROR, v.type);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, v.vresult);

	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue(0, -1, &v));
	CPPUNIT_ASSERT_EQUAL(TT_ERROR, v.type);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, v.vresult);

	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue(0, obj.GetSelectedOutputColumnCount(), &v));
	CPPUNIT_ASSERT_EQUAL(TT_ERROR, v.type);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, v.vresult);


	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("sim"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 1, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("state"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 2, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("soln"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 3, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("dist_x"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 4, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("time"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 5, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("step"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 6, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("pH"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, 7, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("pe"), std::string(v.sVal));

	col = 7;

	// -totals C Ca Na
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("C(mol/kgw)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("Ca(mol/kgw)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("Na(mol/kgw)"), std::string(v.sVal));

	// -molalities CO3-2  CaOH+  NaCO3-
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("m_CO3-2(mol/kgw)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("m_CaOH+(mol/kgw)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("m_NaCO3-(mol/kgw)"), std::string(v.sVal));

	// -activities CO3-2  CaOH+  NaCO3-
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("la_CO3-2"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("la_CaOH+"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("la_NaCO3-"), std::string(v.sVal));

	// -equilibrium_phases Calcite
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("Calcite"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("d_Calcite"), std::string(v.sVal));


	// -saturation_indices CO2(g) Siderite
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("si_CO2(g)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("si_Siderite"), std::string(v.sVal));

	// -gases CO2(g) N2(g)
	//                      pressure "total mol" volume g_CO2(g) g_N2(g)
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("pressure"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("total mol"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("volume"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("g_CO2(g)"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("g_N2(g)"), std::string(v.sVal));

	// -kinetic_reactants Albite Pyrite
	//                               k_Albite dk_Albite k_Pyrite dk_Pyrite
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("k_Albite"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("dk_Albite"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("k_Pyrite"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("dk_Pyrite"), std::string(v.sVal));

	// -solid_solutions CaSO4 SrSO4
	//                              s_CaSO4 s_SrSO4
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("s_CaSO4"), std::string(v.sVal));
	++col;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("s_SrSO4"), std::string(v.sVal));

	for (int i = 0; i < max; ++i)
	{
		std::ostringstream oss1, oss2, oss3;

		// 1.name
		//
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col + 1 + (i*3), &v));
		CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
		oss1 << i + 1 << ".name";
		CPPUNIT_ASSERT_EQUAL( oss1.str(), std::string(v.sVal));

		// 1.type
		//
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col + 2 + (i*3), &v));
		CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
		oss2 << i + 1 << ".type";
		CPPUNIT_ASSERT_EQUAL( oss2.str(), std::string(v.sVal));

		// 1.moles
		//
		CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, col + 3 + (i*3), &v));
		CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
		oss3 << i + 1 << ".moles";
		CPPUNIT_ASSERT_EQUAL( oss3.str(), std::string(v.sVal));
	}

	// sim
	//
	col = 0;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL( 1L, v.lVal );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL( 1L, v.lVal );

	// state
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("i_soln"), std::string(v.sVal) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("react"), std::string(v.sVal) );

	// soln
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL( 1L, v.lVal );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL( 1L, v.lVal );

	// dist_x -- (always as double)
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_EQUAL( -99., v.dVal );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_EQUAL( -99., v.dVal );


	// time -- (always as double)
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_EQUAL( -99., v.dVal );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -DBL_DIG) );

	// step
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL(-99L, v.lVal);
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_LONG, v.type);
	CPPUNIT_ASSERT_EQUAL(1L, v.lVal);


	// pH
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.0, v.dVal, ::pow(10., -DBL_DIG) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.861354, v.dVal, ::pow(10., -6) );

	// pe
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.0, v.dVal, ::pow(10., -DBL_DIG) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
// COMMENT: {8/8/2013 12:26:01 AM}	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.90855, v.dVal, ::pow(10., -1) );

	//
	// -totals C Ca Na
	//

	// C(mol/kgw)
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000e-003, v.dVal, ::pow(10., -DBL_DIG) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1556e-003, v.dVal, ::pow(10., -7) );


	// Ca(mol/kgw)
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000e-003, v.dVal, ::pow(10., -DBL_DIG) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1556e-003, v.dVal, ::pow(10., -7) );


	// Na(mol/kgw)
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000e-003, v.dVal, ::pow(10., -DBL_DIG) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.0000e-003, v.dVal, ::pow(10., -7) );

	// -molalities CO3-2  CaOH+  NaCO3-
	col += 3;

	// -activities CO3-2  CaOH+  NaCO3-
	col += 3;

	// -equilibrium_phases Calcite
	col += 2;

	// -saturation_indices CO2(g) Siderite
	col += 2;

	// -gases CO2(g) N2(g)
	col += 5;

	// -kinetic_reactants Albite Pyrite
	//                               k_Albite dk_Albite k_Pyrite dk_Pyrite
	col += 4;

	// -solid_solutions CaSO4 SrSO4
	//                              s_CaSO4 s_SrSO4
	col += 2;


	// 1.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("Ca+2"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("Calcite"),  std::string(v.sVal));

	// 1.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("equi"),  std::string(v.sVal));

	// 1.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.9177923E-04, v.dVal, ::pow(10., -11) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.8444477E-03, v.dVal, ::pow(10., -10) );

	// 2.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("CaHCO3+"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("Ca+2"),  std::string(v.sVal));

	// 2.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));

	// 2.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.5980e-006, v.dVal, ::pow(10., -10) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1371e-003, v.dVal, ::pow(10., -7) );


	// 3.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("CaCO3"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("CaHCO3+"),  std::string(v.sVal));

	// 3.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));

	// 3.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.2155e-007, v.dVal, ::pow(10., -11) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1598e-005, v.dVal, ::pow(10., -9) );



	// 4.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("CaOH+"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("CaCO3"),  std::string(v.sVal));

	// 4.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type );
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));

	// 4.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.1819e-009, v.dVal, ::pow(10., -13) );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.8668e-006, v.dVal, ::pow(10., -10) );


	// 5.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("CaOH+"),  std::string(v.sVal));

	// 5.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string("aq"),  std::string(v.sVal));

	// 5.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_DOUBLE, v.type );
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 9.7385e-009, v.dVal, ::pow(10., -13) );


	// 6.name
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );

	// 6.type
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );

	// 6.moles
	//
	++col;
	//   i_soln
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
	//   react
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, col, &v));
	CPPUNIT_ASSERT_EQUAL( TT_EMPTY, v.type );
}

void TestIPhreeqc::TestGetSelectedOutputColumnCount(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("llnl.dat"));
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK, EQUILIBRIUM_PHASES(obj, "calcite", 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK, USER_PUNCH(obj, "Ca", 10) );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 62,    obj.GetSelectedOutputColumnCount() );
}

void TestIPhreeqc::TestAddError(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));

	// make sure initialized to empty
	//
	const char* err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(err) );

	// make sure initialized to empty
	//
	const char *expected = "TESTING AddError\n";
	CPPUNIT_ASSERT_EQUAL((size_t)1, obj.AddError(expected));

	// check 1
	//
	err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );

	// check increment
	//
	const char *expected2 = "XXXXXX\n";
	CPPUNIT_ASSERT_EQUAL((size_t)2, obj.AddError(expected2));

	// check concatenation
	//
	err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL( std::string(expected) + std::string(expected2), std::string(err) );


	// clear errors
	//
	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));

	// make sure back to empty
	//
	err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(err) );
}

void TestIPhreeqc::TestAccumulateLine(void)
{
	// TODO
}

void TestIPhreeqc::TestOutputErrorString(void)
{
	// TODO
}

void TestIPhreeqc::TestRunWithCallback(void)
{
	// TODO
}

void TestIPhreeqc::TestRunNoDatabaseLoaded(void)
{
	IPhreeqc obj;

	obj.UnLoadDatabase();
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 1, obj.RunAccumulated() );

	const char expected[] =
		"ERROR: RunAccumulated: No database is loaded\n";
	const char* err = obj.GetErrorString();

	CPPUNIT_ASSERT_EQUAL( std::string(expected), std::string(err) );
}

void TestIPhreeqc::TestCase1(void)
{
	// Case 1 (see do_run)
	// pr.punch == TRUE
	// punch.new_def == FALSE
	// output_isopen(OUTPUT_PUNCH) == FALSE
	// selected_output_on == TRUE

	IPhreeqc obj;

	char SELECTED_OUT[80];
	sprintf(SELECTED_OUT, "selected_1.%lu.out", (unsigned long)obj.Index);

	// remove punch file if it exists
	FileTest sofile(SELECTED_OUT);
	CPPUNIT_ASSERT( sofile.RemoveExisting() );

	// clear all flags
	CPPUNIT_ASSERT_EQUAL( 0,       obj.LoadDatabase("phreeqc.dat") );
	CPPUNIT_ASSERT_EQUAL( false,   obj.PhreeqcPtr->SelectedOutput_map.size() > 0);
	CPPUNIT_ASSERT_EQUAL( TRUE,    obj.PhreeqcPtr->pr.punch);


	CPPUNIT_ASSERT_EQUAL( VR_OK,   SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,   USER_PUNCH(obj, "Ca", 10) );
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,          obj.RunAccumulated() );

	CPPUNIT_ASSERT( sofile.VerifyExists() );
	CPPUNIT_ASSERT( sofile.Size() > 0 );
	CPPUNIT_ASSERT_EQUAL( 62,         obj.GetSelectedOutputColumnCount() );

	CPPUNIT_ASSERT_EQUAL( VR_OK,      SOLUTION(obj, 1.0, 1.0, 1.0) );
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,          obj.RunAccumulated() );

	CPPUNIT_ASSERT( sofile.VerifyExists() );
	CPPUNIT_ASSERT( sofile.Size() > 0 );
	CPPUNIT_ASSERT_EQUAL( 62,         obj.GetSelectedOutputColumnCount() );
}

void TestIPhreeqc::TestCase2(void)
{
	// Case 2 (see do_run)
	// pr.punch == TRUE
	// punch.new_def == TRUE
	// output_isopen(OUTPUT_PUNCH) == FALSE
	// selected_output_on == TRUE

	IPhreeqc obj;

	// remove punch files if they exists
	//
	FileTest sofile("selected.out");
	FileTest c2file("case2.punch");
	CPPUNIT_ASSERT( sofile.RemoveExisting() );
	CPPUNIT_ASSERT( c2file.RemoveExisting() );

	// clear all flags
	CPPUNIT_ASSERT_EQUAL( 0,       obj.LoadDatabase("phreeqc.dat") );
	CPPUNIT_ASSERT_EQUAL( false,   obj.PhreeqcPtr->SelectedOutput_map.size() > 0);
	CPPUNIT_ASSERT_EQUAL( TRUE,    obj.PhreeqcPtr->pr.punch);

	CPPUNIT_ASSERT_EQUAL( VR_OK,   SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,   USER_PUNCH(obj, "Ca", 10) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,   obj.AccumulateLine("-file case2.punch") ); // force have_punch_name to TRUE (see read_selected_ouput)
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,       obj.RunAccumulated() );
	CPPUNIT_ASSERT( sofile.VerifyMissing() );
	CPPUNIT_ASSERT( c2file.VerifyExists() );
	CPPUNIT_ASSERT_EQUAL( 62,      obj.GetSelectedOutputColumnCount() );


	// remove punch files if they exist
	//
	CPPUNIT_ASSERT( sofile.RemoveExisting() );
	CPPUNIT_ASSERT( c2file.RemoveExisting() );

	CPPUNIT_ASSERT_EQUAL( VR_OK,    SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,    USER_PUNCH(obj, "Ca", 10) );
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT( sofile.VerifyMissing() );
	CPPUNIT_ASSERT( c2file.VerifyExists() );
	CPPUNIT_ASSERT_EQUAL( 62,    obj.GetSelectedOutputColumnCount() );
}

void TestIPhreeqc::TestPrintSelectedOutputFalse(void)
{
	IPhreeqc obj;

	// remove punch files if they exists
	//
	if (::FileExists("selected.out"))
	{
		::DeleteFile("selected.out");
	}
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists("selected.out") );

	CPPUNIT_ASSERT_EQUAL( 0, obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// turn off selected output
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PRINT; -selected_output false \n") );

	// run
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0, obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( 0, obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( 0, obj.GetSelectedOutputRowCount() );


	// reset pr.punch to TRUE
	CPPUNIT_ASSERT_EQUAL( 0, obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// run
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(true);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0, obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( 11, obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( 2, obj.GetSelectedOutputRowCount() );
}

void TestIPhreeqc::TestOutputFileOnOff()
{
#if defined(_WIN32)
	int n = ::_fcloseall();
	CPPUNIT_ASSERT_EQUAL(0, n);
#endif

	bool onoff[5];
	onoff[0] = true;   // output_file_on
	onoff[1] = false;  // error_file_on
	onoff[2] = false;  // log_file_on
	onoff[3] = false;  // selected_output_file_on
	onoff[4] = false;  // dump_file_on
	TestFileOnOff("phreeqc.%d.out", onoff[0], onoff[1], onoff[2], onoff[3], onoff[4]);
}

void TestIPhreeqc::TestErrorFileOnOff()
{
	bool onoff[5];
	onoff[0] = false;  // output_file_on
	onoff[1] = true;   // error_file_on
	onoff[2] = false;  // log_file_on
	onoff[3] = false;  // selected_output_file_on
	onoff[4] = false;  // dump_file_on
	TestFileOnOff("phreeqc.%d.err", onoff[0], onoff[1], onoff[2], onoff[3], onoff[4]);
}

void TestIPhreeqc::TestLogFileOnOff()
{
	bool onoff[5];
	onoff[0] = false;  // output_file_on
	onoff[1] = false;  // error_file_on
	onoff[2] = true;   // log_file_on
	onoff[3] = false;  // selected_output_file_on
	onoff[4] = false;  // dump_file_on
	TestFileOnOff("phreeqc.%d.log", onoff[0], onoff[1], onoff[2], onoff[3], onoff[4]);
}

void TestIPhreeqc::TestDumpFileOnOff()
{
	bool onoff[5];
	onoff[0] = false;  // output_file_on
	onoff[1] = false;  // error_file_on
	onoff[2] = false;  // log_file_on
	onoff[3] = false;  // selected_output_file_on
	onoff[4] = true;   // dump_file_on
	TestFileOnOff("dump.%d.out", onoff[0], onoff[1], onoff[2], onoff[3], onoff[4]);
}

void TestIPhreeqc::TestSelOutFileOnOff()
{
	bool onoff[5];
	onoff[0] = false;  // output_file_on
	onoff[1] = false;  // error_file_on
	onoff[2] = false;  // log_file_on
	onoff[3] = true;   // selected_output_file_on
	onoff[4] = false;  // dump_file_on
	TestFileOnOff("selected_1.%d.out", onoff[0], onoff[1], onoff[2], onoff[3], onoff[4]);
}

void TestIPhreeqc::TestFileOnOff(const char* FILENAME_FORMAT, bool output_file_on, bool error_file_on, bool log_file_on, bool selected_output_file_on, bool dump_file_on)
{
	IPhreeqc obj;

	char FILENAME[80];
	sprintf(FILENAME, FILENAME_FORMAT, obj.GetId());

	// remove FILENAME if it exists
	//
	if (::FileExists(FILENAME))
	{
		::DeleteFile(FILENAME);
	}
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(FILENAME) );

	CPPUNIT_ASSERT_EQUAL( 0, obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, DUMP(obj) );

	// run all off
	obj.SetDumpFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(FILENAME) );



	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, DUMP(obj) );

	// run
	obj.SetDumpFileOn(dump_file_on);
	obj.SetErrorFileOn(error_file_on);
	obj.SetLogFileOn(log_file_on);
	obj.SetOutputFileOn(output_file_on);
	obj.SetSelectedOutputFileOn(selected_output_file_on);
	CPPUNIT_ASSERT_EQUAL( 0,    obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( true, ::FileExists(FILENAME) );
	CPPUNIT_ASSERT( ::DeleteFile(FILENAME) );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, DUMP(obj) );

	// run
	obj.SetDumpFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(FILENAME) );

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add selected output block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SELECTED_OUTPUT(obj) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, DUMP(obj) );

	// run
	obj.SetDumpFileOn(dump_file_on);
	obj.SetErrorFileOn(error_file_on);
	obj.SetLogFileOn(log_file_on);
	obj.SetOutputFileOn(output_file_on);
	obj.SetSelectedOutputFileOn(selected_output_file_on);
	CPPUNIT_ASSERT_EQUAL( 0,    obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( true, ::FileExists(FILENAME) );
	CPPUNIT_ASSERT( ::DeleteFile(FILENAME) );
}

VRESULT
SOLUTION(IPhreeqc& obj, double C, double Ca, double Na)
{
	std::ostringstream oss;

	oss << "SOLUTION 1\n";
	oss << "C "  << C  << "\n";
	oss << "Ca " << Ca << "\n";
	oss << "Na " << Na << "\n";

	return obj.AccumulateLine(oss.str().c_str());
}

VRESULT
EQUILIBRIUM_PHASES(IPhreeqc& obj, const char* phase, double si, double amount)
{
	std::ostringstream oss;

	oss << "EQUILIBRIUM_PHASES\n";
	oss << phase << " " << si << " " << amount << "\n";
	return obj.AccumulateLine(oss.str().c_str());
}

VRESULT
USER_PUNCH(IPhreeqc& obj, const char* element, int max)
{
	std::ostringstream oss;

	oss << "USER_PUNCH\n";

	oss << "-head ";
	for (int i = 1; i <= max; ++i)
	{
		oss << i << ".name " << i << ".type " << i << ".moles ";
	}
	oss << "\n";
	oss << "-start" << "\n";
	oss << "10 n = sys(\"" << element << "\"" << ", count, names$, types$, moles)" << "\n";
	oss << "20 n = " << max << "\n";
	oss << "30 if count < " << max << " then n = count" << "\n";
	oss << "40 for i = 1 to n" << "\n";
	oss << "50 PUNCH names$(i), types$(i), moles(i)" << "\n";
	oss << "60 next i" << "\n";
	oss << "70 list" << "\n";
	oss << "-end" << "\n";
	oss << "SELECTED_OUTPUT" << "\n";
	oss << "-totals C Ca Na" << "\n";
	oss << "-molalities CO3-2  CaOH+  NaCO3-" << "\n";
	oss << "-activities CO3-2  CaOH+  NaCO3-" << "\n";
	oss << "-equilibrium_phases Calcite" << "\n";
	oss << "-saturation_indices CO2(g) Siderite" << "\n";
	oss << "-gases CO2(g) N2(g)" << "\n";
	oss << "-kinetic_reactants Albite Pyrite" << "\n";
	oss << "-solid_solutions CaSO4 SrSO4" << "\n";

	return obj.AccumulateLine(oss.str().c_str());
}

VRESULT
USER_PUNCH_NEH(IPhreeqc& obj)
{
	std::ostringstream oss;

	oss << "USER_PUNCH\n";

	oss << "-head head0 head1 head2\n";
	oss << "-start" << "\n";
	oss << "10 PUNCH \"have0\", \"have1\", \"have2\"" << "\n";
	oss << "20 PUNCH \"missing0\", \"missing1\", \"missing2\"" << "\n";
	oss << "-end" << "\n";
	oss << "SELECTED_OUTPUT" << "\n";
	oss << "-totals C Ca Na" << "\n";
	oss << "-molalities CO3-2  CaOH+  NaCO3-" << "\n";
	oss << "-activities CO3-2  CaOH+  NaCO3-" << "\n";
	oss << "-equilibrium_phases Calcite" << "\n";
	oss << "-saturation_indices CO2(g) Siderite" << "\n";
	oss << "-gases CO2(g) N2(g)" << "\n";
	oss << "-kinetic_reactants Albite Pyrite" << "\n";
	oss << "-solid_solutions CaSO4 SrSO4" << "\n";

	return obj.AccumulateLine(oss.str().c_str());
}

VRESULT
SELECTED_OUTPUT(IPhreeqc& obj)
{
	std::ostringstream oss;

	oss << "SELECTED_OUTPUT" << "\n";
	oss << "-totals C Ca Na" << "\n";

	return obj.AccumulateLine(oss.str().c_str());
}

VRESULT
DUMP(IPhreeqc& obj)
{
	std::ostringstream oss;
	oss << "DUMP" << "\n";
	oss << "-solution 1" << "\n";
	return obj.AccumulateLine(oss.str().c_str());
}

void TestIPhreeqc::TestLongHeadings()
{
	char long_header[] = "this_is_a_long_header_0123456789012345678901234567890123456789";
	char long_value[]  = "this_is_a_long_value_01234567890123456789012345678901234567890";

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));

	std::ostringstream oss;
	oss << "SOLUTION" << "\n";

	oss << "SELECTED_OUTPUT" << "\n";
	oss << "-reset false" << "\n";

	oss << "USER_PUNCH" << "\n";
	oss << "-head " <<  long_header << "\n";
	oss << "-start" << "\n";
	oss << "10 PUNCH \"" << long_value << "\"\n";
	oss << "-end" << "\n";
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine(oss.str().c_str()) );

// COMMENT: {10/30/2013 10:39:40 PM}	//{{
// COMMENT: {10/30/2013 10:39:40 PM}	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "SELECTED_OUTPUT" << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "-reset false" << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "USER_PUNCH" << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "-head " <<  long_header << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "-start" << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "10 PUNCH \"" << long_value << "\"\n";
// COMMENT: {10/30/2013 10:39:40 PM}	oss << "-end" << "\n";
// COMMENT: {10/30/2013 10:39:40 PM}	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine(oss.str().c_str()) );
// COMMENT: {10/30/2013 10:39:40 PM}	//}}

	obj.SetOutputFileOn(0);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	CPPUNIT_ASSERT_EQUAL( 0, obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( 2, obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL( 1, obj.GetSelectedOutputColumnCount());

	CVar v;
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.GetSelectedOutputValue(0, 0, &v));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL( std::string(long_header), std::string(v.sVal));

	CVar v1;
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.GetSelectedOutputValue(1, 0, &v1));
	CPPUNIT_ASSERT_EQUAL( TT_STRING, v1.type);
	CPPUNIT_ASSERT_EQUAL( std::string(long_value), std::string(v1.sVal));

	CPPUNIT_ASSERT_EQUAL( VR_INVALIDCOL, obj.GetSelectedOutputValue(1, 1, &v1));
	CPPUNIT_ASSERT_EQUAL( VR_INVALIDROW, obj.GetSelectedOutputValue(2, 0, &v1));
}

void TestIPhreeqc::TestDatabaseKeyword()
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL(1, obj.RunFile("dump"));

	const char *expected =
		"ERROR: Gas not found in PHASES database, Amm(g).\n"
		"ERROR: Calculations terminating due to input errors.\n";

	const char* err = obj.GetErrorString();
	CPPUNIT_ASSERT_EQUAL(std::string(expected), std::string(err));

	const char *exp_warn =
		"WARNING: DATABASE keyword is ignored by IPhreeqc.\n"
		"WARNING: Cell-lengths were read for 1 cells. Last value is used till cell 100.\n"
		"WARNING: No dispersivities were read; disp = 0 assumed.\n"
		"WARNING: Could not find element in database, Amm.\n"
		"	Concentration is set to zero.\n";

	const char* warn = obj.GetWarningString();
	CPPUNIT_ASSERT_EQUAL(std::string(exp_warn), std::string(warn));
}

void TestIPhreeqc::TestDumpString()
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, DUMP(obj) );

	// run
	obj.SetOutputFileOn(0);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	obj.SetDumpStringOn(1);
	CPPUNIT_ASSERT_EQUAL( 0, obj.RunAccumulated() );

	const char* dump_str = obj.GetDumpString();

	CPPUNIT_ASSERT(::strstr(dump_str, "SOLUTION_RAW")                  != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-temp")                         != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-total_h")                      != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-total_o")                      != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-cb")                           != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-totals")                       != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-pH")                           != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-pe")                           != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-mu")                           != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-ah2o")                         != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-mass_water")                   != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-total_alkalinity")             != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-activities")                   != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " C(-4) ")                       != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " E ")                           != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, " O(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "-gammas")                       != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "USE mix none")                  != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "USE reaction none")             != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "USE reaction_temperature none") != NULL);
	CPPUNIT_ASSERT(::strstr(dump_str, "USE reaction_pressure none")    != NULL);
}

void TestIPhreeqc::TestGetDumpStringLineCount(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,          obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::DUMP(obj) );

	// run
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpStringOn() );
	obj.SetDumpStringOn(true);
	CPPUNIT_ASSERT_EQUAL( true,      obj.GetDumpStringOn() );
	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 33,        obj.GetDumpStringLineCount() );
}

void TestIPhreeqc::TestGetDumpStringLine(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,          obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::DUMP(obj) );

	// run
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpStringOn() );

	obj.SetDumpStringOn(true);
	CPPUNIT_ASSERT_EQUAL( true,      obj.GetDumpStringOn() );
	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 33,        obj.GetDumpStringLineCount() );

	int line = 0;

	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "SOLUTION_RAW")                  != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-temp")                         != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-pressure")                     != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-potential")                    != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-total_h")                      != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-total_o")                      != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-cb")                           != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-density")                      != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-totals")                       != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-pH")                           != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-pe")                           != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-mu")                           != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-ah2o")                         != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-mass_water")                   != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-soln_vol")                     != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-total_alkalinity")             != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-activities")                   != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " C(-4) ")                       != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " E ")                           != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), " O(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "-gammas")                       != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "USE mix none")                  != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "USE reaction none")             != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "USE reaction_temperature none") != NULL);
	CPPUNIT_ASSERT(::strstr(obj.GetDumpStringLine(line++), "USE reaction_pressure none")    != NULL);

	// remaining lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(-3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""), std::string(obj.GetDumpStringLine(-4)) );
}

void TestIPhreeqc::TestGetComponentCount(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,         obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// run
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpStringOn() );
	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( (size_t)3, obj.GetComponentCount() );
}

void TestIPhreeqc::TestGetComponent(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,         obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK,     ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// run
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpStringOn() );
	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( (size_t)3, obj.GetComponentCount() );

	CPPUNIT_ASSERT_EQUAL( std::string(""),   std::string(obj.GetComponent(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),   std::string(obj.GetComponent(-1)) );

	CPPUNIT_ASSERT_EQUAL( std::string("C"),  std::string(obj.GetComponent(0)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Ca"), std::string(obj.GetComponent(1)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Na"), std::string(obj.GetComponent(2)) );

	CPPUNIT_ASSERT_EQUAL( std::string(""),   std::string(obj.GetComponent(3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),   std::string(obj.GetComponent(4)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),   std::string(obj.GetComponent(5)) );
}

void TestIPhreeqc::TestListComponents(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat") );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// run
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,     obj.GetDumpStringOn() );
	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );

	std::list< std::string > comps = obj.ListComponents();
	CPPUNIT_ASSERT_EQUAL( (size_t)3, comps.size() );

	std::list< std::string >::iterator it = comps.begin();
	CPPUNIT_ASSERT_EQUAL( std::string("C"),  std::string((*it++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Ca"), std::string((*it++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Na"), std::string((*it++)) );
}

void TestIPhreeqc::TestSetDumpFileName(void)
{
	char DUMP_FILENAME[80];
	sprintf(DUMP_FILENAME, "dump.%06d.out", ::rand());
	if (::FileExists(DUMP_FILENAME))
	{
		::DeleteFile(DUMP_FILENAME);
	}

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// run
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(true);
	obj.SetDumpFileName(DUMP_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,      obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,   ::FileExists(DUMP_FILENAME) );

	std::string lines[33];
	std::ifstream ifs(DUMP_FILENAME);

	size_t i = 0;
	while (i < sizeof(lines)/sizeof(lines[0]) && std::getline(ifs, lines[i]))
	{
		++i;
	}

	int line = 0;
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "SOLUTION_RAW")                  != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-temp")                         != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-pressure")                     != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-potential")                    != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-total_h")                      != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-total_o")                      != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-cb")                           != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-density")                      != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-totals")                       != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-pH")                           != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-pe")                           != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-mu")                           != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-ah2o")                         != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-mass_water")                   != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-soln_vol")                     != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-total_alkalinity")             != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-activities")                   != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " C(-4) ")                       != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " C(4) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " Ca ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " E ")                           != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " H(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " Na ")                          != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), " O(0) ")                        != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "-gammas")                       != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "USE mix none")                  != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "USE reaction none")             != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "USE reaction_temperature none") != NULL);
	CPPUNIT_ASSERT(::strstr(lines[line++].c_str(), "USE reaction_pressure none")    != NULL);

	if (::FileExists(DUMP_FILENAME))
	{
		::DeleteFile(DUMP_FILENAME);
	}
}

void TestIPhreeqc::TestSetOutputFileName(void)
{
	char OUTPUT_FILENAME[80];
	sprintf(OUTPUT_FILENAME, "output.%06d.out", ::rand());
	if (::FileExists(OUTPUT_FILENAME))
	{
		::DeleteFile(OUTPUT_FILENAME);
	}

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat.old"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// run
	obj.SetOutputFileOn(true);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);
	obj.SetOutputFileName(OUTPUT_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,      obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,   ::FileExists(OUTPUT_FILENAME) );

	std::string lines[200];
	std::ifstream ifs(OUTPUT_FILENAME);

	size_t i = 0;
	while (i < sizeof(lines)/sizeof(lines[0]) && std::getline(ifs, lines[i]))
	{
		++i;
	}

	CPPUNIT_ASSERT_EQUAL( (size_t)100, i );

	int line = 0;

	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------------------------")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Reading input data for simulation 1.")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------------------------")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	SOLUTION 1")                                                                    != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	C 1")                                                                           != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	Ca 1")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	Na 1")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	DUMP")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	-solution 1")                                                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "-------------------------------------------")                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Beginning of initial solution calculations.")                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "-------------------------------------------")                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Initial solution 1.	")                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "-----------------------------Solution composition--------")                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	Elements           Molality       Moles")                                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	C                ")                                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	Ca               ")                                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "	Na               ")                                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "----------------------------Description of solution------")                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                                       pH  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                                       pe  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                        Activity of water  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                 Ionic strength (mol/kgw)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                       Mass of water (kg)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                 Total alkalinity (eq/kg)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                       Total CO2 (mol/kg)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                         Temperature (�C)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                  Electrical balance (eq)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), " Percent error, 100*(Cat-|An|)/(Cat+|An|)  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                               Iterations  =  ")                                   != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                                  Total H  = ")                                    != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                                  Total O  = ")                                    != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "----------------------------Distribution of species----------------------------")  != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "                                               Log       Log       Log    mole V") != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   Species          Molality    Activity  Molality  Activity     Gamma    cm�/mol")!= NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   OH- ")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   H+ ")                                                                           != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   H2O ")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "C(-4) ")                                                                           != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CH4 ")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "C(4) ")                                                                            != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   HCO3- ")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CO2 ")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CaHCO3+ ")                                                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CaCO3 ")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CO3-2 ")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   NaHCO3 ")                                                                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   NaCO3- ")                                                                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Ca ")                                                                              != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   Ca+2 ")                                                                         != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CaHCO3+ ")                                                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CaCO3 ")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   CaOH+ ")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "H(0) ")                                                                            != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   H2 ")                                                                           != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Na ")                                                                              != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   Na+ ")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   NaHCO3 ")                                                                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   NaCO3- ")                                                                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   NaOH ")                                                                         != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "O(0) ")                                                                            != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "   O2 ")                                                                           != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------------------Saturation indices-------------------------------")  != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  Phase               SI** log IAP   log K(298 K,   1 atm)")                       != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  Aragonite")                                                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  Calcite")                                                                        != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  CH4(g)")                                                                         != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  CO2(g)")                                                                         != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  H2(g)")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  H2O(g)")                                                                         != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  O2(g)")                                                                          != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "**For a gas, SI = log10(fugacity). Fugacity = pressure * phi / 1 atm.")            != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "  For ideal gases, phi = 1.")                                                      != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------")                                                               != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "End of simulation.")                                                               != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------")                                                               != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------------------------")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "Reading input data for simulation 2.")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "------------------------------------")                                             != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "----------------")                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "End of Run after ")                                                                != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "----------------")                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );
	CPPUNIT_ASSERT( ::strstr(lines[line++].c_str(), "")                                                                                 != NULL );

	if (::FileExists(OUTPUT_FILENAME))
	{
		::DeleteFile(OUTPUT_FILENAME);
	}
}

void TestIPhreeqc::TestOutputStringOnOff(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetOutputStringOn() );

	obj.SetOutputStringOn(true);
	CPPUNIT_ASSERT_EQUAL( true,     obj.GetOutputStringOn() );

	obj.SetOutputStringOn(false);
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetOutputStringOn() );
}

void TestIPhreeqc::TestGetOutputString(void)
{
	char OUTPUT_FILENAME[80];
	sprintf(OUTPUT_FILENAME, "output.%06d.out", ::rand());
	if (::FileExists(OUTPUT_FILENAME))
	{
		::DeleteFile(OUTPUT_FILENAME);
	}
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(OUTPUT_FILENAME) );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// run
	obj.SetOutputFileOn(true);
	obj.SetOutputStringOn(true);

	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);
	obj.SetOutputFileName(OUTPUT_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,      obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,   ::FileExists(OUTPUT_FILENAME) );

	{
		std::ifstream ifs(OUTPUT_FILENAME);
		std::string fline((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		std::string sline(obj.GetOutputString());
		CPPUNIT_ASSERT( sline.size() > 0 );

		CPPUNIT_ASSERT_EQUAL( fline, sline );
	}

	if (::FileExists(OUTPUT_FILENAME))
	{
		::DeleteFile(OUTPUT_FILENAME);
	}
}

void TestIPhreeqc::TestGetOutputStringLineCount(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat.old"));

	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// run
	obj.SetOutputStringOn(false);
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	obj.SetOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 100,    obj.GetOutputStringLineCount() );


	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	obj.SetOutputStringOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );
}

void TestIPhreeqc::TestGetOutputStringLine(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat.old"));

	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// run
	obj.SetOutputStringOn(false);
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	int line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-4)) );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	obj.SetOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 100,    obj.GetOutputStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),                                                std::string(obj.GetOutputStringLine(0))  );
	CPPUNIT_ASSERT_EQUAL( std::string("Reading input data for simulation 1."),                                                std::string(obj.GetOutputStringLine(1))  );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),                                                std::string(obj.GetOutputStringLine(2))  );
	CPPUNIT_ASSERT_EQUAL( std::string("-----------------------------Solution composition------------------------------"),     std::string(obj.GetOutputStringLine(16)) );
	CPPUNIT_ASSERT_EQUAL( std::string("----------------------------Description of solution----------------------------"),     std::string(obj.GetOutputStringLine(24)) );
	CPPUNIT_ASSERT_EQUAL( std::string("----------------------------Distribution of species----------------------------"),     std::string(obj.GetOutputStringLine(40)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------Saturation indices-------------------------------"),     std::string(obj.GetOutputStringLine(73)) );
	CPPUNIT_ASSERT_EQUAL( std::string("End of Run"),                                                                          std::string(obj.GetOutputStringLine(97)).substr(0, 10) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                                                                    std::string(obj.GetOutputStringLine(100)) );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	obj.SetOutputStringOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetOutputStringLineCount() );

	line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetOutputStringLine(-4)) );
}

void TestIPhreeqc::TestSetLogFileName(void)
{
	char LOG_FILENAME[80];
	sprintf(LOG_FILENAME, "log.%06d.out", ::rand());
	if (::FileExists(LOG_FILENAME))
	{
		::DeleteFile(LOG_FILENAME);
	}

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	// run
	obj.SetLogFileOn(true);
	obj.SetErrorFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);
	obj.SetLogFileName(LOG_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,  ::FileExists(LOG_FILENAME) );

	std::string lines[33];
	std::ifstream ifs(LOG_FILENAME);

	size_t i = 0;
	while (i < sizeof(lines)/sizeof(lines[0]) && std::getline(ifs, lines[i]))
	{
		++i;
	}

	CPPUNIT_ASSERT_EQUAL( (size_t)25, i );

	int line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string("-------------------------------------------"), lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Beginning of initial solution calculations."), lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("-------------------------------------------"), lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Initial solution 1.	"),                       lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Iterations in revise_guesses: 2"),             lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of infeasible solutions: 0"),           lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of basis changes: 0"),                  lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of iterations: 6"),                     lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------"),                          lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("End of simulation."),                          lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------"),                          lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("Reading input data for simulation 2."),        lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        lines[line++] );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );
	line++;
	CPPUNIT_ASSERT_EQUAL( std::string("End of Run"),                                  lines[line++].substr(0, 10) );
	line++;
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            lines[line++] );

	if (::FileExists(LOG_FILENAME))
	{
		::DeleteFile(LOG_FILENAME);
	}
}

void TestIPhreeqc::TestLogStringOnOff(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetLogStringOn() );

	obj.SetLogStringOn(true);
	CPPUNIT_ASSERT_EQUAL( true,     obj.GetLogStringOn() );

	obj.SetLogStringOn(false);
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetLogStringOn() );
}

void TestIPhreeqc::TestGetLogString(void)
{
	char LOG_FILENAME[80];
	sprintf(LOG_FILENAME, "log.%06d.out", ::rand());
	if (::FileExists(LOG_FILENAME))
	{
		::DeleteFile(LOG_FILENAME);
	}
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(LOG_FILENAME) );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	// run
	obj.SetLogFileOn(true);
	obj.SetLogStringOn(true);

	obj.SetDumpFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetErrorFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetOutputStringOn(false);
	obj.SetSelectedOutputFileOn(false);

	obj.SetLogFileName(LOG_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,  ::FileExists(LOG_FILENAME) );

	{
		std::ifstream ifs(LOG_FILENAME);
		std::string fline((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());

		std::string sline(obj.GetLogString());
		CPPUNIT_ASSERT( sline.size() > 0 );

		CPPUNIT_ASSERT_EQUAL( fline, sline );
	}

	if (::FileExists(LOG_FILENAME))
	{
		::DeleteFile(LOG_FILENAME);
	}
}

void TestIPhreeqc::TestGetLogStringLineCount(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	// run
	obj.SetLogStringOn(false);
	obj.SetLogFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	obj.SetLogStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 29,    obj.GetLogStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	obj.SetLogStringOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );
}

void TestIPhreeqc::TestGetLogStringLine(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	// run
	obj.SetOutputStringOn(false);
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	int line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-4)) );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	obj.SetLogStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 29,    obj.GetLogStringLineCount() );
	
	line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Reading input data for simulation 1."),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("-------------------------------------------"), std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Beginning of initial solution calculations."), std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("-------------------------------------------"), std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Initial solution 1.	"),                       std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Iterations in revise_guesses: 2"),             std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of infeasible solutions: 0"),           std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of basis changes: 0"),                  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Number of iterations: 6"),                     std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------"),                          std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("End of simulation."),                          std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------"),                          std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("Reading input data for simulation 2."),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("------------------------------------"),        std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string("----------"),                                  std::string(obj.GetLogStringLine(line++)).substr(0, 10) );
	CPPUNIT_ASSERT_EQUAL( std::string("End of Run"),                                  std::string(obj.GetLogStringLine(line++)).substr(0, 10) );
	CPPUNIT_ASSERT_EQUAL( std::string("----------"),                                  std::string(obj.GetLogStringLine(line++)).substr(0, 10) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),                                            std::string(obj.GetLogStringLine(line++)) );

	// add solution block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );

	// add dump block
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::DUMP(obj) );

	// add knobs
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("KNOBS") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("\t-logfile TRUE") );

	obj.SetLogStringOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL( 0,     obj.GetLogStringLineCount() );

	line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-3)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetLogStringLine(-4)) );
}

void TestIPhreeqc::TestSetErrorFileName(void)
{
	char ERR_FILENAME[80];
	sprintf(ERR_FILENAME, "error.%s.out", "TestIPhreeqc-TestSetErrorFileName");
	if (::FileExists(ERR_FILENAME))
	{
		::DeleteFile(ERR_FILENAME);
	}

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	pH	7") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Na	1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	H+ = H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	log_k	0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	// run
	obj.SetErrorFileOn(true);
	obj.SetLogFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);
	obj.SetErrorFileName(ERR_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 1,     obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,  ::FileExists(ERR_FILENAME) );

	std::string lines[100];
	{
		std::ifstream ifs(ERR_FILENAME);

		size_t i = 0;
		while (i < sizeof(lines)/sizeof(lines[0]) && std::getline(ifs, lines[i%100]))
		{
			++i;
		}

		CPPUNIT_ASSERT_EQUAL( (size_t)90, i );
	}

	CPPUNIT_ASSERT_EQUAL( std::string("WARNING: Maximum iterations exceeded, 100"),                                                     lines[0] );
	CPPUNIT_ASSERT_EQUAL( std::string("WARNING: Numerical method failed with this set of convergence parameters."),                     lines[2] );
	CPPUNIT_ASSERT_EQUAL( std::string("ERROR: Numerical method failed on all combinations of convergence parameters, cell/soln/mix 1"), lines[88] );
	CPPUNIT_ASSERT_EQUAL( std::string("Stopping."),                                                                                     lines[89] );

	if (::FileExists(ERR_FILENAME))
	{
		::DeleteFile(ERR_FILENAME);
	}
}

void TestIPhreeqc::TestErrorStringOnOff(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( true,     obj.GetErrorStringOn() );

	obj.SetErrorStringOn(false);
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetErrorStringOn() );

	obj.SetErrorStringOn(true);
	CPPUNIT_ASSERT_EQUAL( true,     obj.GetErrorStringOn() );

	obj.SetErrorStringOn(false);
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetErrorStringOn() );
}

void TestIPhreeqc::TestGetErrorString(void)
{
	char ERR_FILENAME[80];
	sprintf(ERR_FILENAME, "error.%06d.out", ::rand());
	if (::FileExists(ERR_FILENAME))
	{
		::DeleteFile(ERR_FILENAME);
	}
	CPPUNIT_ASSERT_EQUAL( false, ::FileExists(ERR_FILENAME) );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	pH	7") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Na	1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	H+ = H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	log_k	0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	// run
	obj.SetErrorFileOn(true);
	obj.SetErrorStringOn(true);

	obj.SetDumpFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetLogFileOn(false);
	obj.SetOutputFileOn(false);
	obj.SetOutputStringOn(false);
	obj.SetSelectedOutputFileOn(false);

	obj.SetErrorFileName(ERR_FILENAME);
	CPPUNIT_ASSERT_EQUAL( std::string(ERR_FILENAME), std::string(obj.GetErrorFileName()) );

	CPPUNIT_ASSERT_EQUAL( 1,     obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( std::string(ERR_FILENAME), std::string(obj.GetErrorFileName()) );

	CPPUNIT_ASSERT_EQUAL( true,  ::FileExists(ERR_FILENAME) );
	
	{
		std::string fline("ERROR: Numerical method failed on all combinations of convergence parameters, cell/soln/mix 1\n");

		std::string sline(obj.GetErrorString());
		CPPUNIT_ASSERT( sline.size() > 0 );

		CPPUNIT_ASSERT_EQUAL( fline, sline );
	}

	if (::FileExists(ERR_FILENAME))
	{
		::DeleteFile(ERR_FILENAME);
	}
}

void TestIPhreeqc::TestGetErrorStringLineCount(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(     0, obj.GetErrorStringLineCount() );

	CPPUNIT_ASSERT_EQUAL(     0, obj.LoadDatabase("phreeqc.dat") );

	CPPUNIT_ASSERT_EQUAL(     0, obj.GetErrorStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	pH	7") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Na	1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	H+ = H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	log_k	0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	CPPUNIT_ASSERT_EQUAL(  true, obj.GetErrorStringOn() );
	CPPUNIT_ASSERT_EQUAL(     1, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(     1, obj.GetErrorStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	pH	7") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Na	1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	H+ = H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	log_k	0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	obj.SetErrorStringOn(true);
	CPPUNIT_ASSERT_EQUAL(     1, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(     1, obj.GetErrorStringLineCount() );

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	pH	7") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Na	1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	H+ = H+") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	log_k	0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("	Fix_H+ -10 HCl	10") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	obj.SetErrorStringOn(false);
	CPPUNIT_ASSERT_EQUAL(     1, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(     0, obj.GetErrorStringLineCount() );
}

void TestIPhreeqc::TestSetSelectedOutputFileName(void)
{
	char SELOUT_FILENAME[80];
	sprintf(SELOUT_FILENAME, "selected_output.%06d.out", ::rand());
	if (::FileExists(SELOUT_FILENAME))
	{
		::DeleteFile(SELOUT_FILENAME);
	}

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("llnl.dat"));

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(VR_OK, ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010));
	CPPUNIT_ASSERT_EQUAL(VR_OK, ::USER_PUNCH(obj, "Ca", max));

	CPPUNIT_ASSERT_EQUAL(false,  obj.GetOutputFileOn());
	CPPUNIT_ASSERT_EQUAL(false,  obj.GetErrorFileOn());
	CPPUNIT_ASSERT_EQUAL(false,  obj.GetLogFileOn());
	CPPUNIT_ASSERT_EQUAL(false,  obj.GetSelectedOutputFileOn());
	CPPUNIT_ASSERT_EQUAL(false,  obj.GetDumpFileOn());
	CPPUNIT_ASSERT_EQUAL(false,  obj.GetDumpStringOn());

	obj.SetSelectedOutputFileOn(1);
	obj.SetSelectedOutputFileName(SELOUT_FILENAME);

	CPPUNIT_ASSERT_EQUAL( 0,      obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( true,   ::FileExists(SELOUT_FILENAME) );

	/*
	EXPECTED selected.out:
			 sim	       state	        soln	      dist_x	        time	        step	          pH	          pe	           C	          Ca	          Na	     m_CO3-2	     m_CaOH+	    m_NaCO3-	    la_CO3-2	    la_CaOH+	   la_NaCO3-	     Calcite	   d_Calcite	   si_CO2(g)	 si_Siderite	    pressure	   total mol	      volume	    g_CO2(g)	     g_N2(g)	    k_Albite	   dk_Albite	    k_Pyrite	   dk_Pyrite	     s_CaSO4	     s_SrSO4	      1.name	      1.type	     1.moles	      2.name	      2.type	     2.moles	      3.name	      3.type	     3.moles	      4.name	      4.type	     4.moles	      5.name	      5.type	     5.moles	      6.name	      6.type	     6.moles
			   1	      i_soln	           1	         -99	         -99	         -99	           7	           4	 1.0000e-003	 1.0000e-003	 1.0000e-003	 4.2975e-007	 1.1819e-009	 1.1881e-009	-6.4686e+000	-8.9530e+000	-8.9507e+000	 0.0000e+000	 0.0000e+000	     -2.2870	   -999.9990	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	        Ca+2	          aq	 9.9178e-004	     CaHCO3+	          aq	 7.5980e-006	       CaCO3	          aq	 6.2155e-007	       CaOH+	          aq	 1.1819e-009
			   1	       react	           1	         -99	           0	           1	     7.86135	       10.18	 1.1556e-003	 1.1556e-003	 1.0000e-003	 4.2718e-006	 9.7385e-009	 1.1620e-008	-5.4781e+000	-8.0388e+000	-7.9621e+000	 9.8444e-003	-1.5555e-004	     -3.0192	   -999.9990	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	 0.0000e+000	     calcite	        equi	 9.8444e-003	        Ca+2	          aq	 1.1371e-003	     CaHCO3+	          aq	 1.1598e-005	       CaCO3	          aq	 6.8668e-006	       CaOH+	          aq	 9.7385e-009
	*/

	if (::FileExists(SELOUT_FILENAME))
	{
		::DeleteFile(SELOUT_FILENAME);
	}
}

void TestIPhreeqc::TestSelectedOutputStringOnOff(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetSelectedOutputFileOn());

	obj.SetSelectedOutputFileOn(true);
	CPPUNIT_ASSERT_EQUAL( true,     obj.GetSelectedOutputFileOn());

	obj.SetSelectedOutputFileOn(false);
	CPPUNIT_ASSERT_EQUAL( false,    obj.GetSelectedOutputFileOn());
}

void TestIPhreeqc::TestGetSelectedOutputString(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,          obj.LoadDatabase("llnl.dat"));

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(VR_OK,      ::SOLUTION(obj, 1.0, 1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(VR_OK,      ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010));
	CPPUNIT_ASSERT_EQUAL(VR_OK,      ::USER_PUNCH(obj, "Ca", max));

	CPPUNIT_ASSERT_EQUAL(false,      obj.GetOutputFileOn());
	CPPUNIT_ASSERT_EQUAL(false,      obj.GetErrorFileOn());
	CPPUNIT_ASSERT_EQUAL(false,      obj.GetLogFileOn());
	CPPUNIT_ASSERT_EQUAL(false,      obj.GetSelectedOutputFileOn());
	CPPUNIT_ASSERT_EQUAL(false,      obj.GetDumpFileOn());
	CPPUNIT_ASSERT_EQUAL(false,      obj.GetDumpStringOn());

	obj.SetSelectedOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,         obj.RunAccumulated() );

	std::string sline( obj.GetSelectedOutputString() );

	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "sim\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "state\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "soln\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "dist_x\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "time\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "step\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "pH\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "pe\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "C\t")           != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "Ca\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "Na\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "m_CO3-2\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "m_CaOH+\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "m_NaCO3-\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "la_CO3-2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "la_CaOH+\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "la_NaCO3-\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "Calcite\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "d_Calcite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "si_CO2(g)\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "si_Siderite\t") != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "pressure\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "total mol\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "volume\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "g_CO2(g)\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "g_N2(g)\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "k_Albite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "dk_Albite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "k_Pyrite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "dk_Pyrite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "s_CaSO4\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "s_SrSO4\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "1.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "1.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "1.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "2.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "2.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "2.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "3.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "3.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "3.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "4.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "4.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "4.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "5.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "5.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "5.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "6.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "6.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "6.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "\n")            != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "i_soln\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "react\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "Ca+2\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "aq\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "CaHCO3+\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "CaCO3\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "CaOH+\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "Calcite\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(sline.c_str(), "equi\t")        != NULL );
}

void TestIPhreeqc::TestGetSelectedOutputStringLineCount(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	CPPUNIT_ASSERT_EQUAL( 0,      obj.LoadDatabase("llnl.dat") );

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	int max = 6;

	CPPUNIT_ASSERT_EQUAL( VR_OK,  ::SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,  ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010) );
	CPPUNIT_ASSERT_EQUAL( VR_OK,  ::USER_PUNCH(obj, "Ca", max) );

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,  obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,  obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL( false,  obj.GetDumpFileOn());
	CPPUNIT_ASSERT_EQUAL( false,  obj.GetDumpStringOn() );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputStringOn() != 0 );
	obj.SetSelectedOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL( 0,      obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL( 3,      obj.GetSelectedOutputStringLineCount() );
}

void TestIPhreeqc::TestGetSelectedOutputStringLine(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	CPPUNIT_ASSERT_EQUAL( 0,      obj.LoadDatabase("llnl.dat") );

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010) );
	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::USER_PUNCH(obj, "Ca", max) );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetDumpStringOn() );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputStringOn() );

	CPPUNIT_ASSERT_EQUAL(      0, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(      0, obj.GetSelectedOutputStringLineCount() );

	int line = 0;
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(line++)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(line++)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-3)) );

	CPPUNIT_ASSERT_EQUAL( VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010) );
	CPPUNIT_ASSERT_EQUAL( VR_OK, ::USER_PUNCH(obj, "Ca", max) );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputStringOn() != 0 );
	obj.SetSelectedOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL(      0, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(      3, obj.GetSelectedOutputStringLineCount() );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "sim\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "state\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "soln\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dist_x\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "time\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "step\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pH\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pe\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "C\t")           != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Ca\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Na\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_CO3-2\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_CaOH+\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_NaCO3-\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_CO3-2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_CaOH+\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_NaCO3-\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Calcite\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "d_Calcite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_CO2(g)\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_Siderite\t") != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pressure\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "total mol\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "volume\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "g_CO2(g)\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "g_N2(g)\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "k_Albite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dk_Albite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "k_Pyrite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dk_Pyrite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_CaSO4\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_SrSO4\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "1.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "1.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "1.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "2.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "2.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "2.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "3.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "3.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "3.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "4.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "4.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "4.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "5.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "5.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "5.moles\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "6.name\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "6.type\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "6.moles\t")     != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "i_soln\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "Ca+2\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "aq\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "CaHCO3+\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "CaCO3\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "CaOH+\t")       != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "react\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "Calcite\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "equi\t")        != NULL );

	// after obj.GetSelectedOutputStringLineCount() should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount()))     );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount() + 1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount() + 2)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-3)) );
}

void TestIPhreeqc::TestGetSelectedOutputStringLineNotEnoughHeadings(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	CPPUNIT_ASSERT_EQUAL( 0,      obj.LoadDatabase("llnl.dat") );

	CPPUNIT_ASSERT_EQUAL( false,  obj.GetSelectedOutputFileOn() );

	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::SOLUTION(obj, 1.0, 1.0, 1.0) );
	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010) );
	CPPUNIT_ASSERT_EQUAL(  VR_OK, ::USER_PUNCH_NEH(obj) );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetErrorFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetLogFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetDumpFileOn() );
	CPPUNIT_ASSERT_EQUAL(  false, obj.GetDumpStringOn() );

	CPPUNIT_ASSERT_EQUAL(  false, obj.GetSelectedOutputStringOn() != 0 );
	obj.SetSelectedOutputStringOn(true);

	CPPUNIT_ASSERT_EQUAL(      0, obj.RunAccumulated() );
	CPPUNIT_ASSERT_EQUAL(      3, obj.GetSelectedOutputStringLineCount() );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "sim\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "state\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "soln\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dist_x\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "time\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "step\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pH\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pe\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "C\t")           != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Ca\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Na\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_CO3-2\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_CaOH+\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_NaCO3-\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_CO3-2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_CaOH+\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_NaCO3-\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Calcite\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "d_Calcite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_CO2(g)\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_Siderite\t") != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pressure\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "total mol\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "volume\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "g_CO2(g)\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "g_N2(g)\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "k_Albite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dk_Albite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "k_Pyrite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dk_Pyrite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_CaSO4\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_SrSO4\t")     != NULL );	
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "head0\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "head1\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "head2\t")       != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "i_soln\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "have0\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "have1\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "have2\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "missing0\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "missing2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "missing2\t")    != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "react\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "have0\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "have1\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "have2\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "missing0\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "missing2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "missing2\t")    != NULL );

	// after obj.GetSelectedOutputStringLineCount() should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount()))     );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount() + 1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(obj.GetSelectedOutputStringLineCount() + 2)) );

	// negative lines should be empty
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-1)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-2)) );
	CPPUNIT_ASSERT_EQUAL( std::string(""),  std::string(obj.GetSelectedOutputStringLine(-3)) );
}

void TestIPhreeqc::TestLongUser_Punch(void)
{
	// stream tests
	std::ostringstream oss;
	PHRQ_io::fpunchf_helper(&oss, "%2046.2046s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2046, oss.str().size());
	std::string s0(oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(s0));

	oss.clear(); oss.seekp(0);
	PHRQ_io::fpunchf_helper(&oss, "%2047.2047s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2047, oss.str().size());
	std::string s1(oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(s1));

	oss.clear(); oss.seekp(0);
	PHRQ_io::fpunchf_helper(&oss, "%2048.2048s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2048, oss.str().size());
	std::string s2(oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(s2));

	oss.clear(); oss.seekp(0);
	PHRQ_io::fpunchf_helper(&oss, "%2049.2049s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2049, oss.str().size());
	std::string s3(oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(s3));

	oss.clear(); oss.seekp(0);
	PHRQ_io::fpunchf_helper(&oss, "%2050.2050s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2050, oss.str().size());
	std::string s4(oss.str());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(s4));


	// string tests
	std::string str;
	PHRQ_io::fpunchf_helper(&str, "%2046.2046s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2046, str.size());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(str));

	str.clear();
	PHRQ_io::fpunchf_helper(&str, "%2047.2047s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2047, str.size());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(str));

	str.clear();
	PHRQ_io::fpunchf_helper(&str, "%2048.2048s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2048, str.size());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(str));

	str.clear();
	PHRQ_io::fpunchf_helper(&str, "%2049.2049s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2049, str.size());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(str));

	str.clear();
	PHRQ_io::fpunchf_helper(&str, "%2050.2050s", "TEST");
	CPPUNIT_ASSERT_EQUAL((size_t)2050, str.size());
	CPPUNIT_ASSERT_EQUAL(std::string("TEST"), trim(str));

	const char input[] =
		"PRINT\n"
		" -selected_output t\n"
		"SOLUTION\n"
		"SELECTED_OUTPUT\n"
		" -reset false\n"
		"USER_PUNCH\n"
		"1 REM 255 CHARACTER STRING\n"
		"10 temp$ = \"XXXXX123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345\"\n"
		"20 PUNCH temp$\n";

	IPhreeqc obj;
	obj.SetSelectedOutputFileOn(true);
	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(0, obj.RunString(input));
}

void TestIPhreeqc::TestBasicSURF(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL( 0,     obj.LoadDatabase("phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SURFACE_MASTER_SPECIES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfa Surfa") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfb Surfb") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SURFACE_SPECIES") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfa = Surfa") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k 0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfb = Surfb") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k 0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfa + Zn+2 = SurfaZn+2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k  5.") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfb + Zn+2 = SurfbZn+2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k  6.") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfa + Cu+2 = SurfaCu+2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k  4.5") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 Surfb + Cu+2 = SurfbCu+2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("		 log_k  6.5") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SOLUTION 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   pH        8") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   units     mol/kgw") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Fe(3)     1e-2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Zn        1e-4") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Cu        1e-5") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Na        1e-1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Cl        1e-1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("EQUILIBRIUM_PHASES 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("   Fe(OH)3(a) 0 0") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SELECTED_OUTPUT") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("USER_PUNCH") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("    -headings Hfo-Zn Surfa-Zn Surfb-Zn Surfa-Cu Surfb-Cu") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("-start") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("10 PUNCH SURF(\"Zn\",\"Hfo\")") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("20 PUNCH SURF(\"Zn\",\"Surfa\")") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("30 PUNCH SURF(\"Zn\",\"Surfb\")") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("40 PUNCH SURF(\"Cu\",\"Surfa\")") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("50 PUNCH SURF(\"Cu\",\"Surfb\")") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("-end") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("SURFACE 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("    Hfo_sOH Fe(OH)3(a)      equilibrium_phase 0.005  53300") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("    Hfo_wOH Fe(OH)3(a)      equilibrium_phase 0.2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("    Surfa  0.2 100. 2") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("    Surfb  0.1 100. 1") );
	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.AccumulateLine("END") );

	obj.SetOutputStringOn(true);
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpStringOn(false);
	obj.SetDumpFileOn(false);

	CPPUNIT_ASSERT_EQUAL( 0,     obj.RunAccumulated() );

	CPPUNIT_ASSERT_EQUAL(13, obj.GetSelectedOutputColumnCount());
	CPPUNIT_ASSERT_EQUAL(3, obj.GetSelectedOutputRowCount());

	CVar v;

	const int offset = 8;

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, offset + 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Hfo-Zn"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, offset + 1, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Surfa-Zn"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, offset + 2, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Surfb-Zn"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, offset + 3, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Surfa-Cu"), std::string(v.sVal));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, offset + 4, &v));
	CPPUNIT_ASSERT_EQUAL(TT_STRING, v.type);
	CPPUNIT_ASSERT_EQUAL(std::string("Surfb-Cu"), std::string(v.sVal));


	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, offset + 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, offset + 1, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, offset + 2, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, offset + 3, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, offset + 4, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0, v.dVal, ::pow(10., -FLT_DIG) );


	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, offset + 0, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.3861e-005, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, offset + 1, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.7868e-005, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, offset + 2, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 1.8248e-005, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, offset + 3, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.6216e-009, v.dVal, ::pow(10., -FLT_DIG) );

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, offset + 4, &v));
	CPPUNIT_ASSERT_EQUAL(TT_DOUBLE, v.type);
	CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.7201e-008, v.dVal, ::pow(10., -FLT_DIG) );
}

#include <time.h>
void TestIPhreeqc::TestCErrorReporter(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("llnl.dat"));

	int max = 6;

	CPPUNIT_ASSERT_EQUAL(VR_OK, SOLUTION(obj, 1.0, 1.0, 1.0));
	CPPUNIT_ASSERT_EQUAL(VR_OK, EQUILIBRIUM_PHASES(obj, "calcite", 0.0, 0.010));
	CPPUNIT_ASSERT_EQUAL(VR_OK, USER_PUNCH(obj, "Ca", max));

	obj.SetOutputFileOn(0);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunAccumulated());

	//clock_t t0 = clock();
	int nrows = obj.GetSelectedOutputRowCount();
	int ncols = obj.GetSelectedOutputColumnCount();
	CVar var;
	for (int c = 0; c < 4000; ++c)
	{
		for (int row = 0; row < nrows; ++row)
		{
			for (int col = 0; col < ncols; ++col)
			{
				CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(row, col, &var));
			}
		}
	}
	//clock_t t = clock();
	//printf("\ntime = %g\n", double(t - t0));
}

void TestIPhreeqc::TestDelete(void)
{
	const char input[] =
		"SOLUTION 1 # definition of intial condition 1\n"
		"COPY cell 1 7405 # copy cell 1 to placeholder cell with index larger than the number of cells in the model domain\n"
		"END\n"
		"DELETE # delete initial condition 1 to allow for a redefinition of all reactions\n"
		"-cell 1\n"
		"END\n"
		"# define other initial conditions and copy to another placeholder cell\n"
		"\n"
		"COPY cell 7405    1 # copy back from placeholder cell to domain cell 1\n"
		"END\n"
		"MIX    1 # mix according to initial moisture content\n"
		"   1 0.25\n"
		"END\n"
		"RUN_CELLS\n"
		"-cells 1\n"
		"-start_time 0\n"
		"-time_step 0\n"
		"DELETE # remove mix reaction in subsequent runs\n"
		"-mix 1\n"
		"END\n"
		"RUN_CELLS\n"
		"-cells 1\n";

	IPhreeqc obj;

	char OUTPUT_FILE[80];
	sprintf(OUTPUT_FILE, "phreeqc.%lu.out", (unsigned long)obj.Index);

	if (::FileExists(OUTPUT_FILE))
	{
		CPPUNIT_ASSERT(::DeleteFile(OUTPUT_FILE));
	}
	CPPUNIT_ASSERT_EQUAL(false, ::FileExists(OUTPUT_FILE));
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	obj.SetOutputFileOn(0);
	obj.SetErrorFileOn(0);
	obj.SetLogFileOn(0);
	obj.SetSelectedOutputFileOn(0);
	obj.SetDumpFileOn(0);
	CPPUNIT_ASSERT_EQUAL(false, ::FileExists(OUTPUT_FILE));
	CPPUNIT_ASSERT_EQUAL(0,     obj.RunString(input));
	CPPUNIT_ASSERT_EQUAL(false, ::FileExists(OUTPUT_FILE));
	if (::FileExists(OUTPUT_FILE))
	{
		CPPUNIT_ASSERT(::DeleteFile(OUTPUT_FILE));
	}
}

void TestIPhreeqc::TestRunFileMultiPunchOn(void)
{
	FileTest set1("multi_punch_1.sel");
	CPPUNIT_ASSERT( set1.RemoveExisting() );

	FileTest set2("multi_punch_2.sel");
	CPPUNIT_ASSERT( set2.RemoveExisting() );

	FileTest set3("multi_punch_3.sel");
	CPPUNIT_ASSERT( set3.RemoveExisting() );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	obj.SetCurrentSelectedOutputUserNumber(1);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(2);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(3);
	obj.SetSelectedOutputFileOn(true);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT( set1.VerifyExists() );
	CPPUNIT_ASSERT( set2.VerifyExists() );
	CPPUNIT_ASSERT( set3.VerifyExists() );
}

void TestIPhreeqc::TestRunFileMultiPunchOff(void)
{
	FileTest set1("multi_punch_1.sel");
	CPPUNIT_ASSERT( set1.RemoveExisting() );

	FileTest set2("multi_punch_2.sel");
	CPPUNIT_ASSERT( set2.RemoveExisting() );

	FileTest set3("multi_punch_3.sel");
	CPPUNIT_ASSERT( set3.RemoveExisting() );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	obj.SetOutputFileOn(false);
	obj.SetErrorFileOn(false);
	obj.SetLogFileOn(false);
	obj.SetSelectedOutputFileOn(false);
	obj.SetDumpFileOn(false);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT( set1.VerifyMissing() );
	CPPUNIT_ASSERT( set2.VerifyMissing() );
	CPPUNIT_ASSERT( set3.VerifyMissing() );
}

void TestIPhreeqc::TestRunFileMultiPunchSet(void)
{
	FileTest called("XXX.sel");
	CPPUNIT_ASSERT( called.RemoveExisting() );

	FileTest set1("multi_punch_1.sel");
	CPPUNIT_ASSERT( set1.RemoveExisting() );

	FileTest set2("multi_punch_2.sel");
	CPPUNIT_ASSERT( set2.RemoveExisting() );

	FileTest set3("multi_punch_3.sel");
	CPPUNIT_ASSERT( set3.RemoveExisting() );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));

	obj.SetCurrentSelectedOutputUserNumber(1);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(2);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(3);
	obj.SetSelectedOutputFileOn(true);

	obj.SetSelectedOutputFileName(called.GetName().c_str());

	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT( called.VerifyMissing() );

	CPPUNIT_ASSERT( set1.VerifyExists() );
	CPPUNIT_ASSERT( set2.VerifyExists() );
	CPPUNIT_ASSERT( set3.VerifyExists() );
}

void TestIPhreeqc::TestRunFileMultiPunchNoSet(void)
{
	IPhreeqc obj;

	FileTest set("XXX.sel");
	CPPUNIT_ASSERT( set.RemoveExisting() );

	FileTest unset1(obj.sel_file_name(1));
	CPPUNIT_ASSERT( unset1.RemoveExisting() );

	FileTest unset2(obj.sel_file_name(2));
	CPPUNIT_ASSERT( unset2.RemoveExisting() );

	FileTest unset3(obj.sel_file_name(3));
	CPPUNIT_ASSERT( unset3.RemoveExisting() );

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));

	obj.SetCurrentSelectedOutputUserNumber(1);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(2);
	obj.SetSelectedOutputFileOn(true);
	obj.SetCurrentSelectedOutputUserNumber(3);
	obj.SetSelectedOutputFileOn(true);

	obj.SetCurrentSelectedOutputUserNumber(1);
	obj.SetSelectedOutputFileName(set.GetName().c_str());
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch_no_set"));

	CPPUNIT_ASSERT( set.VerifyExists() );
	CPPUNIT_ASSERT( !unset1.VerifyExists() );
	CPPUNIT_ASSERT( unset2.VerifyExists() );
	CPPUNIT_ASSERT( unset3.VerifyExists() );
}

void TestIPhreeqc::TestMultiPunchSelectedOutputStringOn(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	obj.SetSelectedOutputStringOn(true);
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT_EQUAL(6, obj.GetSelectedOutputStringLineCount());

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "sim\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "state\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "soln\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dist_x\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "time\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "step\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pH\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pe\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "reaction\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "temp\t")        != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Alk\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "mu\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "mass_H2O\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "charge\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pct_err\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Na\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "Ca\t")          != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_Na+\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "m_HCO3-\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_Ca+2\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "la_CO3-2\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "CO2(g)\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "d_CO2(g)\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dolomite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "d_dolomite\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_Halite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "pressure\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "total mol\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "volume\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "g_N2(g)\t")     != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "k_Calcite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "dk_Calcite\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_Anhydrite\t") != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "s_Barite\t")    != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "V_TOTAL_C\t")   != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "  8\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), " 10\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(3), " 11\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(4), " 12\t")         != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(5), " 14\t")         != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(1), "react\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "react\t")       != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(3), "i_soln\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(4), "i_soln\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(5), "react\t")       != NULL );

	obj.SetCurrentSelectedOutputUserNumber(2);
	CPPUNIT_ASSERT_EQUAL(9, obj.GetSelectedOutputStringLineCount());

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_Halite\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(0), "si_Calcite\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "Dummy1\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(2), "Dummy2\t")      != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(3), "Dummy1\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(3), "Dummy2\t")      != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(4), "Dummy1\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(4), "Dummy2\t")      != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(5), "Dummy1\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(5), "Dummy2\t")      != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Sum_resid\t")   != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Sum_Delta/U\t") != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "MaxFracErr\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_2\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_2_min\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_2_max\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_3\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_3_min\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Soln_3_max\t")  != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Halite\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(6), "Halite_max\t")  != NULL );

	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(8), "Dummy1\t")      != NULL );
	CPPUNIT_ASSERT( ::strstr(obj.GetSelectedOutputStringLine(8), "Dummy2\t")      != NULL );
}

void TestIPhreeqc::TestMultiPunchCSelectedOutput(void)
{
	CVar var;
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT_EQUAL(6, obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(35, obj.GetSelectedOutputColumnCount());

	// headings
	int ncol = 0;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("sim"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("state"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("soln"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("dist_x"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("time"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("step"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("pH"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("pe"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("reaction"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("temp(C)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Alk(eq/kgw)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("mu"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("mass_H2O"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("charge(eq)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("pct_err"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Na(mol/kgw)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Ca(mol/kgw)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("m_Na+(mol/kgw)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("m_HCO3-(mol/kgw)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("la_Ca+2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("la_CO3-2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("CO2(g)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("d_CO2(g)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("dolomite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("d_dolomite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("si_Halite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("pressure"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("total mol"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("volume"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("g_N2(g)"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("k_Calcite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("dk_Calcite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("s_Anhydrite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("s_Barite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("V_TOTAL_C"), std::string(var.sVal));

	// sim
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 0, &var));   CPPUNIT_ASSERT_EQUAL((long) 8, var.lVal);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 0, &var));   CPPUNIT_ASSERT_EQUAL((long)10, var.lVal);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 0, &var));   CPPUNIT_ASSERT_EQUAL((long)11, var.lVal);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 0, &var));   CPPUNIT_ASSERT_EQUAL((long)12, var.lVal);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 0, &var));   CPPUNIT_ASSERT_EQUAL((long)14, var.lVal);

	// state
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 1, &var));   CPPUNIT_ASSERT_EQUAL(std::string("react"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 1, &var));   CPPUNIT_ASSERT_EQUAL(std::string("react"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 1, &var));   CPPUNIT_ASSERT_EQUAL(std::string("i_soln"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 1, &var));   CPPUNIT_ASSERT_EQUAL(std::string("i_soln"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 1, &var));   CPPUNIT_ASSERT_EQUAL(std::string("react"),  std::string(var.sVal));

	// pH
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.30475, var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.29765, var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.99738, var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 6.99698, var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 7.2942 , var.dVal, ::pow(10., -2) );

	// V_TOTAL_C
#ifdef SKIP_TEST
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 34, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.3729e-003, var.dVal, ::pow(10., -6) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 34, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.3090e-003, var.dVal, ::pow(10., -6) );
#endif
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 34, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0000e+000, var.dVal, ::pow(10., -6) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 34, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.0000e+000, var.dVal, ::pow(10., -6) );
#ifdef SKIP_TEST
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 34, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 4.2784e-003, var.dVal, ::pow(10., -6) );
#endif

	// edge cases
	int r = obj.GetSelectedOutputRowCount();
	int c = obj.GetSelectedOutputColumnCount();
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue(-1,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue( r,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0, -1, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0,  c, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);


	obj.SetCurrentSelectedOutputUserNumber(2);
	CPPUNIT_ASSERT_EQUAL(7, obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(16, obj.GetSelectedOutputColumnCount());

	// headings
	ncol = 0;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("si_Halite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("si_Calcite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("DUMMY_1"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("DUMMY_2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Sum_resid"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Sum_Delta/U"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("MaxFracErr"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2_min"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2_max"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3_min"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3_max"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite_min"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite_max"), std::string(var.sVal));

	// si_Halite
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.70857,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.67087,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.6362,   var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -999.999,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.60092,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -7.60411,  var.dVal, ::pow(10., -2) );

	// si_Calcite
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.692077,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.678847,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.678847,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -999.999,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( -999.999,  var.dVal, ::pow(10., -2) );
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.672429,  var.dVal, ::pow(10., -2) );

	// DUMMY_1
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 2, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 2, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy1"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 2, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy1"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 2, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy1"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 2, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy1"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 2, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy1"), std::string(var.sVal));

	// DUMMY_2
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 3, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 3, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 3, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 3, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 3, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy2"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 3, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Dummy2"), std::string(var.sVal));

	// Sum_resid
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 4, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 4, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 4, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 4, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 4, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 4, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.69E-13,  var.dVal, ::pow(10., log10(3.69E-13)-2) );

	// Sum_Delta/U
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 5, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 5, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 5, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 5, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 5, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 5, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );
	
	// MaxFracErr
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 6, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 6, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 6, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 6, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 6, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_2
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 7, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 7, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 7, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 7, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 7, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 7, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  var.dVal, ::pow(10., -3) );

	// Soln_2_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 8, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 8, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 8, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 8, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 8, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 8, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_2_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 9, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 9, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 9, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 9, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 9, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 9, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_3
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 10, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 10, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 10, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 10, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 10, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 10, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  var.dVal, ::pow(10., -3) );

	// Soln_3_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 11, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 11, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 11, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 11, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 11, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 11, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_3_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 12, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 12, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 12, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 12, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 12, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 12, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Halite
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 13, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 13, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 13, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 13, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 13, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 13, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.001,  var.dVal, ::pow(10., -3) );

	// Halite_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 14, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 14, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 14, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 14, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 14, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 14, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Halite_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 15, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(2, 15, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(3, 15, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(4, 15, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(5, 15, &var));   CPPUNIT_ASSERT_EQUAL(TT_EMPTY,  var.type);
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(6, 15, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// edge cases
	r = obj.GetSelectedOutputRowCount();
	c = obj.GetSelectedOutputColumnCount();
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue(-1,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue( r,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0, -1, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0,  c, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);

	obj.SetCurrentSelectedOutputUserNumber(3);
	CPPUNIT_ASSERT_EQUAL(2, obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(12, obj.GetSelectedOutputColumnCount());

	// headings
	ncol = 0;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Sum_resid"),   std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Sum_Delta/U"), std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("MaxFracErr"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2"),      std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2_min"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_2_max"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3"),      std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3_min"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Soln_3_max"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite"),      std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite_min"),  std::string(var.sVal));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(0, ncol++, &var));   CPPUNIT_ASSERT_EQUAL(std::string("Halite_max"),  std::string(var.sVal));

	// Sum_resid
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 0, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 3.69E-13,  var.dVal, ::pow(10., log10(3.69E-13)-2) );

	// Sum_Delta/U
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 1, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );
	
	// MaxFracErr
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 2, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_2
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 3, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  var.dVal, ::pow(10., -3) );

	// Soln_2_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 4, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_2_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 5, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_3
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 6, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 1,  var.dVal, ::pow(10., -3) );

	// Soln_3_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 7, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Soln_3_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 8, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Halite
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 9, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0.001,  var.dVal, ::pow(10., -3) );

	// Halite_min
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 10, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );

	// Halite_max
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.GetSelectedOutputValue(1, 11, &var));   CPPUNIT_ASSERT_DOUBLES_EQUAL( 0,  var.dVal, ::pow(10., -3) );


	// edge cases
	r = obj.GetSelectedOutputRowCount();
	c = obj.GetSelectedOutputColumnCount();
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue(-1,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, obj.GetSelectedOutputValue( r,  0, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDROW, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0, -1, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, obj.GetSelectedOutputValue( 0,  c, &var));  CPPUNIT_ASSERT_EQUAL(TT_ERROR, var.type);  CPPUNIT_ASSERT_EQUAL(VR_INVALIDCOL, var.vresult);

	CPPUNIT_ASSERT_EQUAL(VR_INVALIDARG, obj.SetCurrentSelectedOutputUserNumber(-1)); 
	CPPUNIT_ASSERT_EQUAL(VR_OK,         obj.SetCurrentSelectedOutputUserNumber(0)); 
}

void TestIPhreeqc::TestGetSelectedOutputCount(void)
{
	CVar var;
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.GetSelectedOutputCount());
	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(0, obj.GetSelectedOutputCount());
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));
	CPPUNIT_ASSERT_EQUAL(3, obj.GetSelectedOutputCount());
}

void TestIPhreeqc::TestGetNthSelectedOutputUserNumber(void)
{
	CVar var;
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT_EQUAL(3, obj.GetSelectedOutputCount());

	CPPUNIT_ASSERT_EQUAL(1, obj.GetNthSelectedOutputUserNumber(0));
	CPPUNIT_ASSERT_EQUAL(2, obj.GetNthSelectedOutputUserNumber(1));
	CPPUNIT_ASSERT_EQUAL(3, obj.GetNthSelectedOutputUserNumber(2));

	// edge cases
	CPPUNIT_ASSERT_EQUAL((int)VR_INVALIDARG, obj.GetNthSelectedOutputUserNumber(-1));
	CPPUNIT_ASSERT_EQUAL((int)VR_INVALIDARG, obj.GetNthSelectedOutputUserNumber(4));
}

void TestIPhreeqc::TestGetCurrentSelectedOutputUserNumber(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(1, obj.GetCurrentSelectedOutputUserNumber());

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(1, obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("multi_punch"));

	CPPUNIT_ASSERT_EQUAL(1,  obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(6,  obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(35, obj.GetSelectedOutputColumnCount());

	obj.SetCurrentSelectedOutputUserNumber(2);
	CPPUNIT_ASSERT_EQUAL(2,  obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(7,  obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(16, obj.GetSelectedOutputColumnCount());

	obj.SetCurrentSelectedOutputUserNumber(3);
	CPPUNIT_ASSERT_EQUAL(3,  obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(2,  obj.GetSelectedOutputRowCount());
	CPPUNIT_ASSERT_EQUAL(12, obj.GetSelectedOutputColumnCount());

	// edge cases
	CPPUNIT_ASSERT_EQUAL(VR_INVALIDARG, obj.SetCurrentSelectedOutputUserNumber(-1)); 
	CPPUNIT_ASSERT_EQUAL(3,             obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(VR_OK,         obj.SetCurrentSelectedOutputUserNumber(0)); 
	CPPUNIT_ASSERT_EQUAL(0,             obj.GetCurrentSelectedOutputUserNumber());

	// unload database
	CPPUNIT_ASSERT_EQUAL(0,             obj.LoadDatabase("../database/phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(1,             obj.GetCurrentSelectedOutputUserNumber());
	CPPUNIT_ASSERT_EQUAL(0,             obj.GetSelectedOutputCount());
}

void TestIPhreeqc::TestMultiSetSelectedOutputFileName(void)
{
	FileTest set1("state.sel");
	CPPUNIT_ASSERT( set1.RemoveExisting() );

	FileTest set2("si.sel");
	CPPUNIT_ASSERT( set2.RemoveExisting() );

	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,             obj.LoadDatabase("../database/phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL(VR_OK,         obj.SetCurrentSelectedOutputUserNumber(1)); 
	obj.SetSelectedOutputStringOn(true);
	obj.SetSelectedOutputFileOn(true);
	obj.SetSelectedOutputFileName(set1.GetName().c_str());

	CPPUNIT_ASSERT_EQUAL(VR_OK,         obj.SetCurrentSelectedOutputUserNumber(2)); 
	obj.SetSelectedOutputStringOn(true);
	obj.SetSelectedOutputFileOn(true);
	obj.SetSelectedOutputFileName(set2.GetName().c_str());

	obj.AccumulateLine("TITLE Temperature dependence of solubility");
	obj.AccumulateLine("      of gypsum and anhydrite             ");
	obj.AccumulateLine("SOLUTION 1 Pure water                     ");
	obj.AccumulateLine("        pH      7.0                       ");
	obj.AccumulateLine("        temp    25.0                      ");
	obj.AccumulateLine("EQUILIBRIUM_PHASES 1                      ");
	obj.AccumulateLine("        Gypsum          0.0     1.0       ");
	obj.AccumulateLine("        Anhydrite       0.0     1.0       ");
	obj.AccumulateLine("REACTION_TEMPERATURE 1                    ");
	obj.AccumulateLine("        25.0 75.0 in 51 steps             ");
	obj.AccumulateLine("SELECTED_OUTPUT 1                         ");
	obj.AccumulateLine("        -temperature                      ");
	obj.AccumulateLine("SELECTED_OUTPUT 2                         ");
	obj.AccumulateLine("        -si     anhydrite  gypsum         ");
	obj.AccumulateLine("END                                       ");

	CPPUNIT_ASSERT_EQUAL(0,             obj.RunAccumulated());

	CPPUNIT_ASSERT( set1.VerifyExists() );
	CPPUNIT_ASSERT( set2.VerifyExists() );
}

void TestIPhreeqc::TestWissmeier20131203(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,      obj.LoadDatabase("../database/phreeqc.dat"));

	obj.AccumulateLine("selected_output 1");
	obj.AccumulateLine("-totals O");
	obj.AccumulateLine("");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output 1");
	obj.AccumulateLine("-totals H");
	obj.AccumulateLine("");
	obj.AccumulateLine("solution");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output");

	// original asserts here
	CPPUNIT_ASSERT_EQUAL(0,      obj.RunAccumulated());

	CPPUNIT_ASSERT_EQUAL( 1,     obj.GetSelectedOutputCount() );

	CPPUNIT_ASSERT_EQUAL( 9,     obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( 2,     obj.GetSelectedOutputRowCount() );
}

void TestIPhreeqc::TestWissmeier20131203_2(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,      obj.LoadDatabase("../database/phreeqc.dat"));

	obj.AccumulateLine("selected_output 22");
	obj.AccumulateLine("-totals O");
	obj.AccumulateLine("");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output 22");
	obj.AccumulateLine("-totals H");
	obj.AccumulateLine("");
	obj.AccumulateLine("solution");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output 22");

	// original asserts here
	CPPUNIT_ASSERT_EQUAL(0,      obj.RunAccumulated());

	CPPUNIT_ASSERT_EQUAL( 1,     obj.GetSelectedOutputCount() );

	CPPUNIT_ASSERT_EQUAL( VR_OK, obj.SetCurrentSelectedOutputUserNumber(22) );
	CPPUNIT_ASSERT_EQUAL( 1,     obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( 2,     obj.GetSelectedOutputRowCount() );
}

void TestIPhreeqc::TestWissmeier20131203_3(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0,      obj.LoadDatabase("../database/phreeqc.dat"));

	obj.AccumulateLine("selected_output 1");
	obj.AccumulateLine("-reset false");
	obj.AccumulateLine("-totals O");
	obj.AccumulateLine("");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output 1");
	obj.AccumulateLine("-reset false");
	obj.AccumulateLine("-totals H");
	obj.AccumulateLine("");
	obj.AccumulateLine("solution");
	obj.AccumulateLine("END");
	obj.AccumulateLine("selected_output");
	obj.AccumulateLine("-reset false");

	// original asserts here
	CPPUNIT_ASSERT_EQUAL(0,      obj.RunAccumulated());

	CPPUNIT_ASSERT_EQUAL( 1,     obj.GetSelectedOutputCount() );

	CPPUNIT_ASSERT_EQUAL( 1,     obj.GetSelectedOutputColumnCount() );
	CPPUNIT_ASSERT_EQUAL( 2,     obj.GetSelectedOutputRowCount() );
}

void TestIPhreeqc::TestKinniburgh20140218(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/wateq4f.dat"));
	CPPUNIT_ASSERT_EQUAL(0, obj.RunFile("kinn20140218"));
}

void TestIPhreeqc::TestGetAccumulatedLines(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 12"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 13"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL( 1,     obj.RunAccumulated() );
		const char expected[] = "solution 12\nsolution 13\n";
		CPPUNIT_ASSERT_EQUAL( std::string(expected), obj.GetAccumulatedLines() );
	}

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 22"));
	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL( 1,     obj.RunAccumulated() );
		const char expected[] = "solution 22\n";
		CPPUNIT_ASSERT_EQUAL( std::string(expected), obj.GetAccumulatedLines() );
	}
}

void TestIPhreeqc::TestGetAccumulatedLinesAfterRunFile(void)
{
	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("../database/wateq4f.dat"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 12"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 13"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(0,     obj.RunFile("kinn20140218"));
		const char expected[] = "";
		CPPUNIT_ASSERT_EQUAL( std::string(expected), obj.GetAccumulatedLines() );
	}
}

void TestIPhreeqc::TestGetAccumulatedLinesAfterRunString(void)
{
	const char input[] =
		"TITLE Example 1.--Add uranium and speciate seawater.\n"
		"SOLUTION 1  SEAWATER FROM NORDSTROM ET AL. (1979)\n"
		"        units   ppm\n"
		"        pH      8.22\n"
		"        pe      8.451\n"
		"        density 1.023\n"
		"        temp    25.0\n"
		"        redox   O(0)/O(-2)\n"
		"        Ca              412.3\n"
		"        Mg              1291.8\n"
		"        Na              10768.0\n"
		"        K               399.1\n"
		"        Fe              0.002\n"
		"        Mn              0.0002  pe\n"
		"        Si              4.28\n"
		"        Cl              19353.0\n"
		"        Alkalinity      141.682 as HCO3\n"
		"        S(6)            2712.0\n"
		"        N(5)            0.29    gfw   62.0\n"
		"        N(-3)           0.03    as    NH4\n"
		"        U               3.3     ppb   N(5)/N(-3)\n"
		"        O(0)            1.0     O2(g) -0.7\n"
		"SOLUTION_MASTER_SPECIES\n"
		"        U       U+4     0.0     238.0290     238.0290\n"
		"        U(4)    U+4     0.0     238.0290\n"
		"        U(5)    UO2+    0.0     238.0290\n"
		"        U(6)    UO2+2   0.0     238.0290\n"
		"SOLUTION_SPECIES\n"
		"        #primary master species for U\n"
		"        #is also secondary master species for U(4)\n"
		"        U+4 = U+4\n"
		"                log_k          0.0\n"
		"        U+4 + 4 H2O = U(OH)4 + 4 H+\n"
		"                log_k          -8.538\n"
		"                delta_h        24.760 kcal\n"
		"        U+4 + 5 H2O = U(OH)5- + 5 H+\n"
		"                log_k          -13.147\n"
		"                delta_h        27.580 kcal\n"
		"        #secondary master species for U(5)\n"
		"        U+4 + 2 H2O = UO2+ + 4 H+ + e-\n"
		"                log_k          -6.432\n"
		"                delta_h        31.130 kcal\n"
		"        #secondary master species for U(6)\n"
		"        U+4 + 2 H2O = UO2+2 + 4 H+ + 2 e-\n"
		"                log_k          -9.217\n"
		"                delta_h        34.430 kcal\n"
		"        UO2+2 + H2O = UO2OH+ + H+\n"
		"                log_k          -5.782\n"
		"                delta_h        11.015 kcal\n"
		"        2UO2+2 + 2H2O = (UO2)2(OH)2+2 + 2H+\n"
		"                log_k          -5.626\n"
		"                delta_h        -36.04 kcal\n"
		"        3UO2+2 + 5H2O = (UO2)3(OH)5+ + 5H+\n"
		"                log_k          -15.641\n"
		"                delta_h        -44.27 kcal\n"
		"        UO2+2 + CO3-2 = UO2CO3\n"
		"                log_k          10.064\n"
		"                delta_h        0.84 kcal\n"
		"        UO2+2 + 2CO3-2 = UO2(CO3)2-2\n"
		"                log_k          16.977\n"
		"                delta_h        3.48 kcal\n"
		"        UO2+2 + 3CO3-2 = UO2(CO3)3-4\n"
		"                log_k          21.397\n"
		"                delta_h        -8.78 kcal\n"
		"PHASES\n"
		"        Uraninite\n"
		"        UO2 + 4 H+ = U+4 + 2 H2O\n"
		"        log_k          -3.490\n"
		"        delta_h        -18.630 kcal\n"
		"END\n"
		"\n";

	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 12"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 13"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(0,     obj.RunString(input));
		const char expected[] = "";
		CPPUNIT_ASSERT_EQUAL( std::string(expected), obj.GetAccumulatedLines() );
	}
}

void TestIPhreeqc::TestPBasicStopThrow(void)
{
	IPhreeqc obj;

	CPPUNIT_ASSERT_EQUAL(0, obj.LoadDatabase("../database/phreeqc.dat"));

	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("SOLUTION 1 # Mine water from Bain et al., 2001"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("USER_PRINT"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("10 print -(0.006^0.9)"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("20 print TOTMOL(\"H\"), TOTMOLE(\"H\"), TOTMOLES(\"H\")"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("110 print (-0.2)^3"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("120 print (-0.2)^3.0"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("130 print (-0.2)^-2"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("140 print (-0.2)^-3"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("150 print -0.2^2.2"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("END"));

	obj.SetOutputFileOn(true);
	CPPUNIT_ASSERT_EQUAL(    5, obj.RunAccumulated() );
}

void TestIPhreeqc::TestEx10(void)
{
	const char input[] =
		"TITLE Example 10.--Solid solution of strontianite and aragonite.\n"
		"PHASES\n"
		"        Strontianite\n"
		"                SrCO3 = CO3-2 + Sr+2\n"
		"                log_k           -9.271\n"
		"        Aragonite\n"
		"                CaCO3 = CO3-2 + Ca+2\n"
		"                log_k           -8.336\n"
		"END\n"
		"SOLID_SOLUTIONS 1\n"
		"        Ca(x)Sr(1-x)CO3 \n"
		"                -comp1   Aragonite       0 \n"
		"                -comp2   Strontianite    0 \n"
		"                -Gugg_nondim   3.43    -1.82\n"
		"END\n"
		"SOLUTION 1\n"
		"        -units mmol/kgw\n"
		"        pH 5.93 charge\n"
		"        Ca      3.932\n"
		"        C       7.864\n"
		"EQUILIBRIUM_PHASES 1\n"
		"        CO2(g) -0.01265 10\n"
		"        Aragonite\n"
		"SAVE solution 1\n"
		"END\n"
		"#\n"
		"#  Total of 0.00001 to 0.005 moles of SrCO3 added\n"
		"#\n"
		"USE solution 1\n"
		"USE solid_solution 1\n"
		"REACTION 1\n"
		"        SrCO3   1.0\n"
		"        .005 in 500 steps \n"
		"PRINT\n"
		"        -reset false\n"
		"        -echo true\n"
		"        -user_print true\n"
		"USER_PRINT\n"
		"-start\n"
		"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
		"  20 if sum = 0 THEN GOTO 110\n"
		"  30 xb = S_S(\"Strontianite\")/sum\n"
		"  40 xc = S_S(\"Aragonite\")/sum\n"
		"  50 PRINT \"Simulation number:    \", SIM_NO\n"
		"  60 PRINT \"Reaction step number: \", STEP_NO\n"
		"  70 PRINT \"SrCO3 added:          \", RXN\n"
		"  80 PRINT \"Log Sigma pi:         \", LOG10 (ACT(\"CO3-2\") * (ACT(\"Ca+2\") + ACT(\"Sr+2\")))\n"
		"  90 PRINT \"XAragonite:           \", xc\n"
		" 100 PRINT \"XStrontianite:        \", xb\n"
		" 110 PRINT \"XCa:                  \", TOT(\"Ca\")/(TOT(\"Ca\") + TOT(\"Sr\"))\n"
		" 120 PRINT \"XSr:                  \", TOT(\"Sr\")/(TOT(\"Ca\") + TOT(\"Sr\"))\n"
		" 130 PRINT \"Misc 1:               \", MISC1(\"Ca(x)Sr(1-x)CO3\")\n"
		" 140 PRINT \"Misc 2:               \", MISC2(\"Ca(x)Sr(1-x)CO3\")\n"
		"-end\n"
		"SELECTED_OUTPUT\n"
		"        -file ex10.sel\n"
		"        -reset false\n"
		"        -reaction true\n"
		"USER_PUNCH\n"
		"-head   lg_SigmaPi X_Arag X_Stront X_Ca_aq X_Sr_aq mol_Misc1 mol_Misc2 \\n"
		"     mol_Arag mol_Stront\n"
		"-start\n"
		"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
		"  20 if sum = 0 THEN GOTO 60\n"
		"  30 xb = S_S(\"Strontianite\")/(S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
		"  40 xc = S_S(\"Aragonite\")/(S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
		"  50 REM Sigma Pi\n"
		"  60 PUNCH LOG10(ACT(\"CO3-2\") * (ACT(\"Ca+2\") + ACT(\"Sr+2\")))\n"
		"  70 PUNCH xc                                 # Mole fraction aragonite\n"
		"  80 PUNCH xb                                 # Mole fraction strontianite\n"
		"  90 PUNCH TOT(\"Ca\")/(TOT(\"Ca\") + TOT(\"Sr\"))  # Mole aqueous calcium\n"
		"  100 PUNCH TOT(\"Sr\")/(TOT(\"Ca\") + TOT(\"Sr\")) # Mole aqueous strontium\n"
		"  110 x1 = MISC1(\"Ca(x)Sr(1-x)CO3\")\n"
		"  120 x2 = MISC2(\"Ca(x)Sr(1-x)CO3\")\n"
		"  130 if (xb < x1 OR xb > x2) THEN GOTO 250\n"
		"  140    nc = S_S(\"Aragonite\")\n"
		"  150    nb = S_S(\"Strontianite\")\n"
		"  160    mol2 = ((x1 - 1)/x1)*nb + nc\n"
		"  170    mol2 = mol2 / ( ((x1 -1)/x1)*x2 + (1 - x2))\n"
		"  180    mol1 = (nb - mol2*x2)/x1\n"
		"  190    REM                                 # Moles of misc. end members if in gap\n"
		"  200    PUNCH mol1\n"
		"  210    PUNCH mol2\n"
		"  220    GOTO 300\n"
		"  250    REM                                 # Moles of misc. end members if not in gap\n"
		"  260    PUNCH 1e-10\n"
		"  270    PUNCH 1e-10\n"
		"  300 PUNCH S_S(\"Aragonite\")                 # Moles aragonite\n"
		"  310 PUNCH S_S(\"Strontianite\")              # Moles Strontianite\n"
		"-end\n"
		"USER_GRAPH Example 10\n"
		"        -headings x_Aragonite  x_Srontianite\n"
		"        -chart_title \"Aragonite-Strontianite Solid Solution\"\n"
		"        -axis_titles \"Log(SrCO3 added, in moles)\" \"Log(Mole fraction of component)\"\n"
		"        -axis_scale x_axis -5 1 1 1\n"
		"        -axis_scale y_axis -5 0.1 1 1\n"
		"        -connect_simulations true\n"
		"        -start\n"
		"  10 sum = (S_S(\"Strontianite\") + S_S(\"Aragonite\"))\n"
		"  20 IF sum = 0 THEN GOTO 70\n"
		"  30 xb = S_S(\"Strontianite\")/ sum\n"
		"  40 xc = S_S(\"Aragonite\")/ sum\n"
		"  50 PLOT_XY LOG10(RXN), LOG10(xc), line_w = 2, symbol_size = 0\n"
		"  60 PLOT_XY LOG10(RXN), LOG10(xb), line_w = 2, symbol_size = 0\n"
		"  70 rem\n"
		"  -end\n"
		"END     \n"
		"#\n"
		"#  Total of 0.005 to 0.1 moles of SrCO3 added\n"
		"#\n"
		"USE solution 1\n"
		"USE solid_solution 1\n"
		"REACTION 1\n"
		"        SrCO3   1.0\n"
		"        .1 in 20 steps \n"
		"END     \n"
		"#\n"
		"#  Total of 0.1 to 10 moles of SrCO3 added\n"
		"#\n"
		"USE solution 1\n"
		"USE solid_solution 1\n"
		"REACTION 1\n"
		"        SrCO3   1.0\n"
		"        10.0 in 100 steps \n"
		"END     \n";

	IPhreeqc obj;
	CPPUNIT_ASSERT_EQUAL(0,     obj.LoadDatabase("phreeqc.dat"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 12"));
	CPPUNIT_ASSERT_EQUAL(VR_OK, obj.AccumulateLine("solution 13"));

	for (int i = 0; i < 10; ++i)
	{
		CPPUNIT_ASSERT_EQUAL(0,     obj.RunString(input));
		const char expected[] = "";
		CPPUNIT_ASSERT_EQUAL( std::string(expected), obj.GetAccumulatedLines() );
	}
}
