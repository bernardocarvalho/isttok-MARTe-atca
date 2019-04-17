
#include "MagneticsGAM.h"
#include "math.h"


OBJECTLOADREGISTER(MagneticsGAM, "$Id: $")


//  ******** Default constructor ***********************************
MagneticsGAM::MagneticsGAM() {
	this->SignalsInputInterface = NULL;
	this->SignalsOutputInterface = NULL;
}


// ********* Destructor ********************************************
MagneticsGAM::~MagneticsGAM()
{

}


//{ ********* Initialise the module ********************************
bool MagneticsGAM::Initialise(ConfigurationDataBase& cdbData) {

	CDBExtended cdb(cdbData);

	int i,j;

	// read config file section: magnetic_probes
	if (!cdb.ReadInt32(usectime_to_wait_for_starting_operation, "usectime_to_wait_for_starting_operation"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s usectime_to_wait_for_starting_operation", this->Name());
		return False;
	}
	else AssertErrorCondition(Information, "MagneticsGAM::Initialise: usectime_to_wait_for_starting_operation = %d", usectime_to_wait_for_starting_operation);
	if (!cdb.ReadInt32(i, "magnetic_radial_bool"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s magnetic_radial_bool", this->Name());
		return False;
	}
	else
	{
		magnetic_radial_bool = (bool)i;
		AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_radial_bool = %d", magnetic_radial_bool);
	}
	if (!cdb.ReadInt32(i, "magnetic_vertical_bool"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s magnetic_vertical_bool", this->Name());
		return False;
	}
	else
	{
		magnetic_vertical_bool = (bool)i;
		AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_vertical_bool = %d", magnetic_vertical_bool);
	}
	if (!cdb.ReadInt32(i, "magnetic_module_correction_bool"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s magnetic_module_correction_bool", this->Name());
		return False;
	}
	else
	{
		magnetic_module_correction_bool = (bool)i;
		AssertErrorCondition(Information, "MagneticsGAM::Initialise:  = %d", magnetic_module_correction_bool);
	}


	if (!cdb->Move("MirnovArrayDescription"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"+MARTe.+ISTTOK_RTTh.+magnetic_probes.MirnovArrayDescription\"", this->Name());
		return False;
	}
	if (!cdb.ReadInt32(NumberOfProbes, "NumberOfProbes"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfProbes", this->Name());
		return False;
	}
	else	AssertErrorCondition(Information, "MagneticsGAM::Initialise: NumberOfProbes = %d", NumberOfProbes);

	if (NumberOfProbes > 0) {
		magnetic_Angles = new float[NumberOfProbes];
		magnetic_Calibration = new float[NumberOfProbes];

		if (!cdb.ReadFloatArray(magnetic_Angles, (int *)(&NumberOfProbes), 1, "Angles"))
		{
			AssertErrorCondition(InitialisationError, "ReadWaveformFiles: Could not read magnetic_Angles");
			return False;
		}
		else for (i = 0; i<NumberOfProbes; i++) AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_Angles[%d] = %f", i, magnetic_Angles[i]);
		if (!cdb.ReadFloatArray(magnetic_Calibration, (int *)(&NumberOfProbes), 1, "Calibration"))
		{
			AssertErrorCondition(InitialisationError, "ReadWaveformFiles: Could not read magnetic_Calibration");
			return False;
		}
		else for (i = 0; i<NumberOfProbes; i++) AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_Calibration[%d] = %f", i, magnetic_Calibration[i]);
	}
	else {
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfProbes lower than 1", this->Name());
		return False;
	}

	cdb->MoveToFather();

	if (!cdb->Move("Measurements"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"+MARTe.+ISTTOK_RTTh.+magnetic_probes.Measurements\"", this->Name());
		return False;
	}
	if (!cdb.ReadInt32(NumberOfMeasurements, "NumberOfMeasurements"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfMeasurements", this->Name());
		return False;
	}
	else	AssertErrorCondition(Information, "MagneticsGAM::Initialise: NumberOfMeasurements = %d", NumberOfMeasurements);

	if (NumberOfMeasurements > 0) {
		ProbeNumbers = new int[NumberOfMeasurements];

		if (!cdb.ReadInt32Array(ProbeNumbers, (int *)(&NumberOfMeasurements), 1, "ProbeNumbers"))
		{
			AssertErrorCondition(InitialisationError, "ReadWaveformFiles: Could not read ProbeNumbers");
			return False;
		}
		else for (i = 0; i<NumberOfMeasurements; i++) AssertErrorCondition(Information, "MagneticsGAM::Initialise: ProbeNumbers[%d] = %d", i, ProbeNumbers[i]);
	}
	else {
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfMeasurements lower than 1", this->Name());
		return False;
	}

	cdb->MoveToFather();

	//Added for module offset correction
	if (!cdb->Move("ModuleOffsetCorrectionLSBusec"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"+MARTe.+ISTTOK_RTTh.+magnetic_probes.ModuleOffsetCorrectionLSBusec\"", this->Name());
		return False;
	}
	if (!cdb.ReadInt32(NumberOfModules, "NumberOfModules"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfModules", this->Name());
		return False;
	}
	else	AssertErrorCondition(Information, "MagneticsGAM::Initialise: NumberOfModules = %d", NumberOfMeasurements);

	if (NumberOfModules > 0) {
		magnetic_Offset_slope = new float[NumberOfModules];

		if (!cdb.ReadFloatArray(magnetic_Offset_slope, (int *)(&NumberOfModules), 1, "OffsetCalibration"))
		{
			AssertErrorCondition(InitialisationError, "ReadWaveformFiles: Could not OffsetCalibration");
			return False;
		}
		else for (i = 0; i<NumberOfModules; i++) AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_Offset_slope[%d] = %f", i, magnetic_Offset_slope[i]);
	}
	else {
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfModules lower than 1", this->Name());
		return False;
	}

	if (NumberOfModules > 0) {
		magnetic_Polarity_calibration = new float[NumberOfModules];

		if (!cdb.ReadFloatArray(magnetic_Polarity_calibration, (int *)(&NumberOfModules), 1, "PolarityCalibration"))
		{
			AssertErrorCondition(InitialisationError, "ReadWaveformFiles: Could not read PolarityCalibration");
			return False;
		}
		else for (i = 0; i<NumberOfModules; i++) AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_Polarity_calibration[%d] = %f", i, magnetic_Polarity_calibration[i]);
	}
	else {
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s NumberOfModules lower than 1", this->Name());
		return False;
	}

	cdb->MoveToFather();

	// Create the signal interfaces
	if (!AddInputInterface(this->SignalsInputInterface, "MagneticsGAMInputInterface"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s failed to add the MagneticsGAMInputInterface", this->Name());
		return False;
	}
	if (!AddOutputInterface(this->SignalsOutputInterface, "MagneticsGAMOutputInterface"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s failed to add the MagneticsGAMOutputInterface", this->Name());
		return False;
	}

	//	INPUT SIGNALS (interface)
	if (!cdb->Move("input_signals"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"input_signals\"", this->Name());
		return False;
	}
	int number_of_signals_to_read = 16;
	FString *CDB_move_to;
	FString *SignalType;
	CDB_move_to = new FString[number_of_signals_to_read];
	SignalType = new FString[number_of_signals_to_read];
	for (i = 0; i<number_of_signals_to_read - 1; i++)
		CDB_move_to[i].Printf("Channel_%d", i);
	CDB_move_to[number_of_signals_to_read - 1].Printf("time");
	for (i = 0; i<number_of_signals_to_read; i++) {

		if (!cdb->Move(CDB_move_to[i].Buffer()))
		{
			AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"%s\"", this->Name(), CDB_move_to[i].Buffer());
			return False;
		}
		if (cdb->Exists("SignalType"))
		{
			FString signalName;
			cdb.ReadFString(SignalType[i], "SignalType");
		}
		if (cdb->Exists("SignalName"))
		{
			FString SignalName;
			cdb.ReadFString(SignalName, "SignalName");
			AssertErrorCondition(Information, "MagneticsGAM::Initialise: Added signal = %s", SignalName.Buffer());

			if (!this->SignalsInputInterface->AddSignal(SignalName.Buffer(), SignalType[i].Buffer()))
			{
				AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s failed to add signal", this->Name());
				return False;
			}
		}
		cdb->MoveToFather();
	}

	cdb->MoveToFather();

	//	OUTPUT SIGNALS (interface)
	if (!cdb->Move("output_signals"))
	{
		AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"output_signals\"", this->Name());
		return False;
	}

	number_of_signals_to_read = 56; //56
	CDB_move_to = new FString[number_of_signals_to_read];
	SignalType = new FString[number_of_signals_to_read];
	CDB_move_to[0].Printf("magnetic_probes_r");
	CDB_move_to[1].Printf("magnetic_probes_z");
	CDB_move_to[2].Printf("magnetic_probes_plasma_current");
	CDB_move_to[3].Printf("ADC_magnetic_WO_corrctd_0");
	CDB_move_to[4].Printf("ADC_magnetic_WO_corrctd_1");
	CDB_move_to[5].Printf("ADC_magnetic_WO_corrctd_2");
	CDB_move_to[6].Printf("ADC_magnetic_WO_corrctd_3");
	CDB_move_to[7].Printf("ADC_magnetic_WO_corrctd_4");
	CDB_move_to[8].Printf("ADC_magnetic_WO_corrctd_5");
	CDB_move_to[9].Printf("ADC_magnetic_WO_corrctd_6");
	CDB_move_to[10].Printf("ADC_magnetic_WO_corrctd_7");
	CDB_move_to[11].Printf("ADC_magnetic_WO_corrctd_8");
	CDB_move_to[12].Printf("ADC_magnetic_WO_corrctd_9");
	CDB_move_to[13].Printf("ADC_magnetic_WO_corrctd_10");
	CDB_move_to[14].Printf("ADC_magnetic_WO_corrctd_11");
	CDB_move_to[15].Printf("Magnetics_flux_corrctd_0");
	CDB_move_to[16].Printf("Magnetics_flux_corrctd_1");
	CDB_move_to[17].Printf("Magnetics_flux_corrctd_2");
	CDB_move_to[18].Printf("Magnetics_flux_corrctd_3");
	CDB_move_to[19].Printf("Magnetics_flux_corrctd_4");
	CDB_move_to[20].Printf("Magnetics_flux_corrctd_5");
	CDB_move_to[21].Printf("Magnetics_flux_corrctd_6");
	CDB_move_to[22].Printf("Magnetics_flux_corrctd_7");
	CDB_move_to[23].Printf("Magnetics_flux_corrctd_8");
	CDB_move_to[24].Printf("Magnetics_flux_corrctd_9");
	CDB_move_to[25].Printf("Magnetics_flux_corrctd_10");
	CDB_move_to[26].Printf("Magnetics_flux_corrctd_11");
	CDB_move_to[27].Printf("Magnetics_ext_flux_0");
	CDB_move_to[28].Printf("Magnetics_ext_flux_1");
	CDB_move_to[29].Printf("Magnetics_ext_flux_2");
	CDB_move_to[30].Printf("Magnetics_ext_flux_3");
	CDB_move_to[31].Printf("Magnetics_ext_flux_4");
	CDB_move_to[32].Printf("Magnetics_ext_flux_5");
	CDB_move_to[33].Printf("Magnetics_ext_flux_6");
	CDB_move_to[34].Printf("Magnetics_ext_flux_7");
	CDB_move_to[35].Printf("Magnetics_ext_flux_8");
	CDB_move_to[36].Printf("Magnetics_ext_flux_9");
	CDB_move_to[37].Printf("Magnetics_ext_flux_10");
	CDB_move_to[38].Printf("Magnetics_ext_flux_11");
	CDB_move_to[39].Printf("Magnetics_R_uncorrctd");
	CDB_move_to[40].Printf("Magnetics_z_uncorrctd");
	CDB_move_to[41].Printf("Magnetics_Ip_corrctd");
	CDB_move_to[42].Printf("RMSE_mirnv");
	//CDB_move_to[43].Printf("RMSE_Ifil");
	CDB_move_to[43].Printf("SumIfil");
	CDB_move_to[44].Printf("Magnetics_SVD_recons_0");
	CDB_move_to[45].Printf("Magnetics_SVD_recons_1");
	CDB_move_to[46].Printf("Magnetics_SVD_recons_2");
	CDB_move_to[47].Printf("Magnetics_SVD_recons_3");
	CDB_move_to[48].Printf("Magnetics_SVD_recons_4");
	CDB_move_to[49].Printf("Magnetics_SVD_recons_5");
	CDB_move_to[50].Printf("Magnetics_SVD_recons_6");
	CDB_move_to[51].Printf("Magnetics_SVD_recons_7");
	CDB_move_to[52].Printf("Magnetics_SVD_recons_8");
	CDB_move_to[53].Printf("Magnetics_SVD_recons_9");
	CDB_move_to[54].Printf("Magnetics_SVD_recons_10");
	CDB_move_to[55].Printf("Magnetics_SVD_recons_11");
	//CDB_move_to[56].Printf("SumIfil");

	for (i = 0; i<number_of_signals_to_read; i++) {

		if (!cdb->Move(CDB_move_to[i].Buffer()))
		{
			AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s Could not move to \"%s\"", this->Name(), CDB_move_to[i].Buffer());
			return False;
		}

		if (cdb->Exists("SignalType"))
		{
			FString signalName;
			cdb.ReadFString(SignalType[i], "SignalType");
		}
		if (cdb->Exists("SignalName"))
		{
			FString SignalName;
			cdb.ReadFString(SignalName, "SignalName");
			AssertErrorCondition(Information, "MagneticsGAM::Initialise: Added signal = %s", SignalName.Buffer());

			if (!this->SignalsOutputInterface->AddSignal(SignalName.Buffer(), SignalType[i].Buffer()))
			{
				AssertErrorCondition(InitialisationError, "MagneticsGAM::Initialise: %s failed to add signal", this->Name());
				return False;
			}
		}

		cdb->MoveToFather();
	}

	cdb->MoveToFather();

	// Initialise the accumulators
	this->allmirnv_prim = new float[this->NumberOfProbes];
	this->allmirnv_hor = new float[this->NumberOfProbes];
	this->allmirnv_vert = new float[this->NumberOfProbes];
	this->ADC_values = new float[this->NumberOfProbes];
	this->ADC_WO = new float[this->NumberOfProbes];
	this->ADC_WO_Wb = new float[this->NumberOfProbes];
	
	
	this->corrected_probes = new float[this->NumberOfMeasurements];
	this->magnetic_Offset_zero = new float[this->NumberOfModules];
	for (i = 0; i < this->NumberOfProbes; i++) {
		this->ADC_values[i] = 0.0;
		this->ADC_WO[i] = 0.0;
		this->ADC_WO_Wb[i] = 0.0;
		this->corrected_probes[i] = 0.0;
		this->magnetic_Offset_zero[i] = 0.0;
		this->allmirnv_prim[i]=0.0;
		this->allmirnv_vert[i] = 0.0;
		this->allmirnv_hor[i] = 0.0;
	}
	magnetic_field_sum = 0.0;
	magnetic_field_sum_corr = 0.0;

	//ACHTUNG ACHTUNG!!! 0.1 if 100us and 0.01 if 1000us
	// Correct Offsets factor - values Bits/ms -> bits/100us
	for (i = 0; i < this->NumberOfMeasurements; i++) {
		this->magnetic_Offset_slope[i] = this->magnetic_Offset_slope[i] * 1;
	}

	// Initialise the auxiliary probe position values [m]
	this->n_samples = 0;
	this->major_radius = 0.46;
	this->probe_radius = 0.0935;
	this->clip_limit = 0.085; // -clip_limit < output r and z position < +clip_limit

	//Some constantants
	this->Area = 4.9e-5; //[m^2]
	this->Nvoltas = 50; //
	this->MAgPerm = 4*3.16159e-7; //[V*s/A*m]
	this->ADCconst = 10/((2^17)*2e6);
	this->Ncoils = 12;

//ADC fatores
this->ADC_fact = (float[12]) {0.8605*1e-11 ,0.8582*1e-11 ,0.8518*1e-11 ,0.8633*1e-11 ,0.8583*1e-11 ,0.8590*1e-11 ,0.8616*1e-11 ,0.8610*1e-11 ,0.8580*1e-11 ,0.8608*1e-11 ,0.8576*1e-11 ,0.8653*1e-11 };
			AssertErrorCondition(Information,"MagneticsGAM:: Initialise: ADC_fact[5]: ");
			
			//AssertErrorCondition(Information, "MagneticsGAM::Initialise: magnetic_Polarity_calibration[%d] = %f", i, magnetic_Polarity_calibration[i]);


////////////////////////////////////////////////////







/////////////////// slopes
	
	
this ->slope_avrg= (float[12]) { 0,0,0,0,0,0,0,0,0,0,0,0};

	this->slopes = new float*[this->NumberOfProbes];
	for (i = 0; i<12; i++) {
		//this->slopes[i] = new float[29];
		this->slopes[i] = new float[35];

	}

	/*float slopes[12][29]={
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }};*/
			
	float slopes[12][35]={
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 ,0,0,0,0,0,0},
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
			{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }};
			
	
	// Inicilização valores das correntes
	float prim_meas=0.0;
	float hor_meas=0.0;
	float vert_meas=0.0;
			
///////////////// External fluxes subtraction!!!!!!  /////////////////////////// 
	//Lets load initial conditions of the state space models (prim and vert 1 state, hor 4 states) in the state vectors
	this ->x_prim=(float[12]){-4.2670*1e-6,	-3.6560*1e-5, 9.6572*1e-5,	-3.9196*1e-4,	-2.5227*1e-4,	1.0415*1e-4,	-2.1947*1e-5,	1.2819*1e-4,	-8.63235*1e-6, -9.2918*1e-5,	-7.3422*1e-5,	-1.2908*1e-5};	
	this ->x_vert=(float[12]){-0.0508*1e-3, 0.0089*1e-3, -0.0211*1e-3,    0.1913*1e-3,    0.0071*1e-3,   -0.0163*1e-3,   -0.0022*1e-3,-0.0175*1e-3,  0.0359*1e-3,0.0699*1e-3, 0.0458*1e-3,-0.0329*1e-3};

	this -> x_hor_cpy=0.0;

	this->x_test=(float[48]){-0.0003,	-0.0013,	-0.0116,	-0.0028,
							 0.00005, 	 0.0043,	-0.0005,	 0.0028,
							-0.0001,	-0.0108,	-0.0015,	 0.0077,
							 0.0015, 	 0.0111,	-0.0034,	-0.0042,
							 0.0002,	 0.0034,	 0.0121, 	 0.0041,
							-0.0002,	-0.0026,	-0.0123, 	 0.0037
							-0.0001,	-0.0004,	 0.0002,	 0.0014,
							-0.0002,    -0.0019,	-0.0135,  	-0.0044,
							 0.0001,  	 0.0015,  	 0.0135,	-0.0045,
							 0.0001,   	 0.0122, 	-0.0042,  	-0.0026,
							 0.0003,   	 0.0089, 	-0.0012,   	 0.0104,
							-0.00022,  	-0.00103,	-0.01175,  	 0.00346};


	
	this->x_hor= new float*[12];
	for (i = 0; i<12; i++) {
		this->x_hor[i] = new float[4];
	}
	float x_hor[12][4]={{-0.0003,	-0.0013,	-0.0116,	-0.0028},
						{0.00005,	 0.0043,	-0.0005, 	 0.0028},
						{-0.0001,	-0.0108,	-0.0015,	 0.0077},
						{ 0.0015, 	 0.0111,	-0.0034,	-0.0042},
						{0.0002,	 0.0034,	 0.0121, 	 0.0041},
						{-0.0002,	-0.0026,	-0.0123, 	 0.0037},
						{-0.0001,	-0.0004,	 0.0002,	 0.0014},
						{-0.0002,   -0.0019,	-0.0135,  	-0.0044},
						{0.0001,  	 0.0015,  	 0.0135,	-0.0045},
						{0.0001,   	 0.0122, 	-0.0042,  	-0.0026},
						{0.0003,   	 0.0089, 	-0.0012,   	 0.0104},
						{-0.0002,  	-0.0010,	-0.0118,  	 0.0035}};
			     
		this->x_hor_buff= new float*[12];
		for (i = 0; i<12; i++) {
		this->x_hor_buff[i] = new float[4];
		}
			     
	  float x_hor_buff[12][4]={	{-0.0003,	-0.0013,	-0.0116,	-0.0028},
								{0.00005, 	 0.0043,	-0.0005,	 0.0028},
								{-0.0001,	-0.0108,	-0.0015,	 0.0077},
								{ 0.0015,	 0.0111,	-0.0034,	-0.0042},
								{0.0002,	 0.0034,	 0.0121, 	 0.0041},
								{-0.0002,	-0.0026,	-0.0123,	 0.0037},
								{-0.0001,	-0.0004,  	 0.0002,     0.0014},
								{-0.0002,	-0.0019, 	-0.0135,    -0.0044},
								{0.0001,	 0.0015,     0.0135,    -0.0045},
								{0.0001, 	 0.0122,	-0.0042,    -0.0026},
								{0.0003,	 0.0089, 	-0.0012,     0.0104},
								{-0.0002,	-0.0010,	-0.0118,     0.0035}};		
			
		
		this->x_hor_vec=(float[48]){-0.0003,	-0.0013,	-0.0116,	-0.0028,
									 0.000049,	 0.00434,	-0.000516, 	 0.002797,
									-0.0001,	-0.0108,	-0.0015,	 0.0077,
									 0.0015, 	 0.0111,	-0.0034,	-0.0042,
									 0.0002,	 0.0034,	 0.0121, 	 0.0041,
									-0.0002,	-0.0026,	-0.0123, 	 0.0037,
									-0.000535,	-0.0004,	 0.0001785,	 0.001442,
									-0.0002,    -0.0019,	-0.0135,  	-0.0044,
									 0.00014,  	 0.00154,  	 0.01353,	-0.004522,
									 0.0001,   	 0.0122, 	-0.0042,  	-0.0026,
									 0.0003,   	 0.0089, 	-0.0012,   	 0.0104,
									-0.00022,  	-0.00103,	-0.01175,  	 0.00346};
	
		
		this->x_buff_hor=(float[48]){	-0.0003,	-0.0013,	-0.0116,	-0.0028,
									 0.000049,	 0.00434,	-0.000516, 	 0.002797,
									-0.0001,	-0.0108,	-0.0015,	 0.0077,
									 0.0015, 	 0.0111,	-0.0034,	-0.0042,
									 0.0002,	 0.0034,	 0.0121, 	 0.0041,
									-0.0002,	-0.0026,	-0.0123, 	 0.0037,
									-0.000535,	-0.0004,	 0.0001785,	 0.001442,
									-0.0002,    -0.0019,	-0.0135,  	-0.0044,
									 0.00014,  	 0.00154,  	 0.01353,	-0.004522,
									 0.0001,   	 0.0122, 	-0.0042,  	-0.0026,
									 0.0003,   	 0.0089, 	-0.0012,   	 0.0104,
									-0.00022,  	-0.00103,	-0.01175,  	 0.00346};
		
		
			
		// Load A matrices
		
		this ->A_prim=(float[12]){0.9938 , 0.9961, 0.9949,0.9967 , 0.9955,0.9948,0.9963, 0.9969 , 0.99726 ,0.9980 ,0.9965,0.9956};
		this ->A_vert=(float[12]){0.9965,0.9954 ,0.9948 , 0.9958 , 0.9957, 0.9974 ,0.9979,0.9956,0.9954,0.9956, 0.9970,0.9971};				
		
		this->A_hor= new float**[12]();
	for (i = 0; i<12; i++) {
		this->A_hor[i] = new float*[4]();
		for (j = 0; j < 4; j++)
			this->A_hor[i][j]=new float[4]();
	}
	
	float A_hor[12][4][4]= {{{0.9721,-0.0326,-0.0061,0.0111}, 	{-0.0652,0.9102,-0.0437,0.0786},	{-0.0021,-0.0113,0.8091,0.5950},	{-0.0251,-0.0883,-0.5616, 0.7994}},
							{{0.9890,-0.0052, 0.0037, 0.0024},	{-0.0668,0.7655,0.4834,0.0621},		{0.0444,0.1091,0.5870,-1.0826},		{0.0347, 0.2852,-0.3127,-0.2216}},
							{{0.9911,-0.0040,-0.0030,-0.0025},	{-0.0326,0.8415,-0.4706,-0.0508},	{0.0145,0.4454,0.8093, -0.3695},	{-0.0338,0.1428,0.3406,0.8775}},
							{{0.9759,-0.0401,-0.0430,-0.0229},	{-0.0048,0.8243,-0.5626,0.0195},	{0.0596,0.5560,0.8088,-0.1765},		{-0.0188,0.0708,0.1653,0.9535}	},
							{{0.9998,-0.0145,-0.0039,0.0087},	{0.0073,0.9215,-0.1037,0.1497},		{-0.0006,0.0304,0.8199, 0.5662},	{-0.0114,-0.1889,-0.5493, 0.8112}	},
							{{0.9994,-0.0167,-0.0031,-0.0068},	{0.0251,0.8960,-0.0582,-0.0925},	{-0.0026,0.0015,0.8130,-0.5863},	{0.0063,0.1171,0.5652,0.8039}},
							{{0.9996,-0.0208,0.0112,0.0027},	{0.0126,0.9118,0.0834,0.0073},		{0.0131,0.2135,0.0647,-0.7607},		{0.0005,-0.0272,-0.2133,-0.0849}	},
							{{0.9943,-0.0259,-0.0041, 0.0083},	{-0.0087, 0.9473,-0.0247, 0.0483},	{-0.0049,-0.0160,0.8105,0.5887},	{-0.0175, -0.0719,-0.5718, 0.8019}	},
							{{ 0.9854, -0.0100,-0.0024, -0.0041},{-0.0418,0.9476, -0.0552,-0.0867},	{0.0040,0.0030,0.8126,-0.5872},		{0.0119 , 0.1033,0.5672, 0.8079}	},
							{{0.9921, -0.0019 ,-0.0029 ,0.0014},{ -0.0100,0.8107,-0.5821, -0.0003},	{0.0074,0.5790, 0.8081 ,0.1007},	{0.0428,-0.0296,-0.0934 ,0.9092}	},
							{{0.9849,-0.0085,-0.0062 , 0.0045},	{-0.0446 , 0.8552 ,-0.4275,0.0607},	{0.0224,0.3938,0.8094, 0.4208},		{0.0317, -0.1666,-0.3879 , 0.8713}	},
							{{0.9929 ,-0.0441,-0.0068,-0.0120},	{-0.0137,0.8813,-0.0429,-0.0549},	{-0.0061, -0.0155,0.7965,-0.6088},	{0.0166 , 0.0665 ,0.5261 , 0.7680}	} };
	
	this->A_hor_vec=(float[192]){0.9721,-0.0326,-0.0061,0.0111, 	-0.0652,0.9102,-0.0437,0.0786,		-0.0021,-0.0113,0.8091,0.5950,		-0.0251,-0.0883,-0.5616, 0.7994,
							0.98899,-0.005224, 0.00369, 0.00235,	-0.0668,0.76546,0.4834,0.062133,	0.04444,0.10913,0.58697,-1.08256,	 0.03467, 0.28522,-0.3127,-0.22157,
							0.9911,-0.0040,-0.0030,-0.0025,			-0.0326,0.8415,-0.4706,-0.0508,		 0.0145,0.4454,0.8093, -0.3695,		-0.0338,0.1428,0.3406,0.8775,
							0.9759,-0.0401,-0.0430,-0.0229,			-0.0048,0.8243,-0.5626,0.0195,		 0.0596,0.5560,0.8088,-0.1765,		-0.0188,0.0708,0.1653,0.9535,
							0.9998,-0.0145,-0.0039,0.0087,			 0.0073,0.9215,-0.1037,0.1497,		-0.0006,0.0304,0.8199, 0.5662,		-0.0114,-0.1889,-0.5493, 0.8112,
							0.9994,-0.0167,-0.0031,-0.0068,			 0.0251,0.8960,-0.0582,-0.0925,		-0.0026,0.0015,0.8130,-0.5863,		 0.0063,0.1171,0.5652,0.8039,
							0.99963,-0.0208,0.011171,0.00269,		 0.012551,0.91183,0.08343,0.007323,	 0.01309,0.21349,0.064681,-0.7607,	 0.000463,-0.02717,-0.21327,-0.084942,
							0.9943,-0.0259,-0.0041, 0.0083,			-0.0087, 0.9473,-0.0247, 0.0483,	-0.0049,-0.0160,0.8105,0.5887,		-0.0175, -0.0719,-0.5718, 0.8019,
							0.9854, -0.0100,-0.0024, -0.0041,		-0.0418,0.9476, -0.0552,-0.0867,	 0.0040,0.0030,0.8126,-0.5872,		 0.0119 , 0.1033,0.5672, 0.8079,
							0.9921, -0.0019 ,-0.0029 ,0.0014, 		-0.0100,0.8107,-0.5821, -0.0003,	 0.0074,0.5790, 0.8081 ,0.1007,		 0.0428,-0.0296,-0.0934 ,0.9092,
							0.9849,-0.0085,-0.0062 , 0.0045,		-0.0446 , 0.8552 ,-0.4275,0.0607,	 0.0224,0.3938,0.8094, 0.4208,		 0.0317, -0.1666,-0.3879 , 0.8713,
							0.9929 ,-0.044079,-0.006832,-0.01195,	-0.01365,0.8813,-0.0429,-0.05485,	-0.006125, -0.0155,0.79645,-0.6088,	 0.01659 , 0.06648 ,0.5261 , 0.76796};
	


	//this->A_hor_vec=(float[16]){0.9721,-0.0326,-0.0061,0.0111, 	-0.0652,0.9102,-0.0437,0.0786,		-0.0021,-0.0113,0.8091,0.5950,		-0.0251,-0.0883,-0.5616, 0.799};
	

//Load B matrices
	this ->B_prim=(float[12]){-0.3714*1e-5,-0.1609*1e-5,-0.2301*1e-5, 0.1228*1e-5 , 0.2129*1e-5 ,0.2721*1e-5 , 0.1622*1e-5, 0.1193*1e-5 ,0.113422*1e-5,-0.0536*1e-5,-0.1395*1e-5,-0.2660*1e-5};	
	this ->B_vert=(float[12]){-0.0737*1e-5 ,-0.0934*1e-5 ,-0.1150*1e-5 , 0.0823*1e-5 ,   0.0855*1e-5 ,  0.0411*1e-5 ,0.0313*1e-5 , 0.0893*1e-5 , 0.0978*1e-5 ,-0.0806*1e-5 ,-0.0473*1e-5 ,-0.0486*1e-5};

	this->B_hor= new float*[12];
	for (i = 0; i<12; i++) {
		this->B_hor[i] = new float[4];
	}
	float B_hor[12][4]={{-0.0001, 		-0.000006,		 0.0019,		-0.0003},
						{-0.000004, 	 0.0002,  		 0.0038,		 0.0040},
						{-0.0082*1e-3,   0.4674*1e-3,    0.4001*1e-3,  	-0.5271*1e-3},
						{-0.0416*1e-3,   0.2097*1e-3,    0.1572*1e-3, 	-0.1167*1e-3},
						{-0.0151*1e-3,   0.0303*1e-3,    0.5044*1e-3,   -0.1020*1e-3},
						{-0.0244*1e-3,  -0.0719*1e-3,    0.8074*1e-3,    0.1968*1e-3},
						{0.0001,  		 0.0003,  		-0.0044,  		-0.0046},
						{ 0.0293*1e-3,   0.0212*1e-3,   -0.6610*1e-3,    0.2805*1e-3},
						{0.00001, 		-0.0001,		-0.0012,  		-0.0003},
						{0.0052*1e-3,	-0.2924*1e-3,   -0.1013*1e-3, 	-0.2193*1e-3},
						{0.0193*1e-3, 	-0.3272*1e-3,   -0.5058*1e-3, 	-0.4650*1e-3},
						{0.00004487, 		-0.00000308,	    -0.002449,   		-0.0001197}};
	
						
	this->B_hor_vec=(float[48]) {-6.8*1e-5, 	-6.4*1e-6,	     0.0019,		-2.58*1e-4,
								-0.00000439, 	 0.0002489,  	 0.00383,		 0.00397,
								-0.0082*1e-3,    0.4674*1e-3,    0.4001*1e-3,  	-0.5271*1e-3,
								-0.0416*1e-3,    0.2097*1e-3,    0.1572*1e-3, 	-0.1167*1e-3,
								-0.0151*1e-3,    0.0303*1e-3,    0.5044*1e-3,   -0.1020*1e-3,
								-0.0244*1e-3,   -0.0719*1e-3,    0.8074*1e-3,    0.1968*1e-3,
								 0.0000567,  	 0.00025896,  	-0.004413,  	-0.0045883,
								 0.0293*1e-3,    0.0212*1e-3,   -0.6610*1e-3,    0.2805*1e-3,
								 0.000015, 		-0.000089,		-0.0012,  		-0.00034,
								 0.0052*1e-3,	-0.2924*1e-3,   -0.1013*1e-3, 	-0.2193*1e-3,
								 0.0193*1e-3, 	-0.3272*1e-3,   -0.5058*1e-3, 	-0.4650*1e-3,
								 0.00004487, 	-0.00000308,	-0.002449,   	-0.0001197};



//Load C matrices
	this ->C_prim=(float[12]){0.0999*1e-3,0.1566*1e-3,0.0397*1e-3 , 0.0789*1e-3 , 0.1139*1e-3 , 0.1109*1e-3, 0.1292*1e-3,  0.1344*1e-3 , 0.074446*1e-3 ,0.0534*1e-3 , 0.1600*1e-3,0.0886*1e-3};
	this ->C_vert=(float[12]){0.0864*1e-3, 0.1852*1e-3,  0.0815*1e-3, 0.1314*1e-3,0.1429*1e-3,0.0853*1e-3,0.1165*1e-3, 0.1880*1e-3, 0.0894*1e-3, 0.1057*1e-3,0.1770*1e-3, 0.0791*1e-3};


	this->C_hor= new  float*[12];
	for (i = 0; i<12; i++) {
		this->C_hor[i] = new  float[4];
	}
	
float C_hor[12][4]={{ 0.2379*1e-4,  0.0087*1e-4,   0.0012*1e-4,  0.0013*1e-4 },
					{0.1081*1e-3,   0.0001*1e-3,   0.0001*1e-3,  0.0001e-3 },
					{0.7142*1e-4,   0.0021*1e-4,  -0.0008*1e-4, -0.0001e-4},
					{0.1182*1e-4,   0.0030*1e-4,  -0.0017*1e-4,  0.0004e-4},
					{0.3215*1e-4,   0.0021*1e-4,   0.0007*1e-4,  0.0013e-4 },
					{0.3228*1e-4,   0.0015*1e-4,   0.0005*1e-4, -0.0009e-4},
					{0.3824*1e-4,   0.0031*1e-4,   0.0011*1e-4,  0.0013e-4},
					{0.1744*1e-4, 	0.0009*1e-4,   0.0003*1e-4,  0.0005e-4},
					{0.2916*1e-4,   0.0016*1e-4,   0.0013*1e-4, -0.0005e-4 },
					{0.8227*1e-4,  	0.0002*1e-4,  -0.0008*1e-4,  0.0001e-4},
					{0.5518*1e-4,  	0.0038*1e-4,  -0.0012*1e-4,  0.0005e-4 },
					{0.1707*1e-4, 	0.0025*1e-4,   0.0005*1e-4, -0.0006e-4}};
					
					
					
		
					
					
					
			this->	C_hor_vec=(float[48]){ 0.2379*1e-4,  0.0087*1e-4,   0.0012*1e-4,  0.0013*1e-4 ,
										  0.10807*1e-3,  0.000076*1e-3, 0.000142*1e-3,0.0000665e-3 ,
											0.7142*1e-4,   0.0021*1e-4,  -0.0008*1e-4, -0.0001e-4,
											0.1182*1e-4,   0.0030*1e-4,  -0.0017*1e-4,  0.0004e-4,
											0.3215*1e-4,   0.0021*1e-4,   0.0007*1e-4,  0.0013e-4 ,
											0.3228*1e-4,   0.0015*1e-4,   0.0005*1e-4, -0.0009e-4,
											0.38237*1e-4,  0.003142*1e-4, 0.0011299*1e-4, 0.0012515*1e-4,
											0.1744*1e-4, 	0.0009*1e-4,   0.0003*1e-4,  0.0005e-4,
											0.2916*1e-4,   0.0016*1e-4,   0.0013*1e-4, -0.00046*1e-4 ,
											0.8227*1e-4,  	0.0002*1e-4,  -0.0008*1e-4,  0.0001e-4,
											0.5518*1e-4,  	0.0038*1e-4,  -0.0012*1e-4,  0.0005e-4,
											0.17073*1e-4, 	0.00252*1e-4,   0.0004647*1e-4, -0.0006451*1e-4};	
															
/*
// Initialization of the variables for calculating the centroid position for multifilament model
// 7 filaments * 12 coils
// radius 5 [cm] from central filament to radial filaments
	
this-> Mpf_SVD=(float[84]){ 472086.187804917,	-4052047.62807573,	32703579.1848192,	-69579011.4842266,	53936244.5981449,	-14625341.5996303,	-14625341.5996320,	53936244.5981467,	-69579011.4842287,	32703579.1848217,	-4052047.62807835, 	472086.187807036,
				-316709.787104180,	519760.850846756,	-3873209.37066971,	8605169.84682676,	-6698686.11099274,	1821327.17941657,	1821327.17941678,	-6698686.11099296,	8605169.84682702,	-3873209.37067001,	519760.850847079, 	-316709.787104442,
				 87572.5960975867,	594952.653746323,	-4666762.49801214,	9841834.01697765,	-7593437.88764413,	2023655.61913615,	2070594.23630447,	-7476191.61519410,	9823449.28637081,	-4900604.26840064,	390889.046074736,	28882.1479281171,
				-116870.066911669,	702403.027615122,	-5532800.73445480,	11800871.0803144,	-9190923.13011351,	2613106.92855291,	2527393.43079230,	-9418773.85954442,	11684658.8220455,	-5434320.77170507,	815172.401765702,	-83997.6701786653,
				-64386.4596194328,	716978.164436526,	-6038724.51758130,	12916600.7926723,	-9823791.54520333,	2502031.14386129,	2502031.14386160,	-9823791.54520366,	12916600.7926727,	-6038724.51758175,	716978.164437008,	-64386.4596198232,
				-83997.6701783051,	815172.401765258,	-5434320.77170466,	11684658.8220452,	-9418773.85954412,	2527393.43079201,	2613106.92855320,	-9190923.13011381,	11800871.0803148,	-5532800.73445522,	702403.027615567,	-116870.066912029,
				 28882.1479284166,	390889.046074366,	-4900604.26840029,	9823449.28637052,	-7476191.61519385,	2070594.23630423,	2023655.61913639,	-7593437.88764438,	9841834.01697794,	-4666762.49801249,	594952.653746693,	87572.5960972870};
	
					
this-> Mfp=(float[84]){ -1.43676746989355*1e-06,	-3.50297447689559*1e-06,	-1.59432882220592*1e-06,	-9.07753536601227*1e-07,	-7.63816088771978*1e-07,	-1.03564689815154*1e-06,	-2.18441630395307*1e-06,
			-1.57621934794434*1e-06,	-2.69259341848311*1e-06,	-1.39114476121602*1e-06,	-9.23068023384328*1e-07,	-8.84005456272177*1e-07,	-1.40111127004305*1e-06,	-3.46486130426331*1e-06,
			-1.85652982126111*1e-06,	-2.26120807255429*1e-06,	-1.39382382931709*1e-06,	-1.06609554873689*1e-06,	-1.18036237305197*1e-06,	-2.28602171554116*1e-06,	-4.21360234587828*1e-06,
			-2.25902290915744*1e-06,	-2.12824503200163*1e-06,	-1.54098776944301*1e-06,	-1.36572542664713*1e-06,	-1.80299627067539*1e-06,	-4.17140909972325*1e-06,	-3.55052980292162*1e-06,
			-2.70099739689445*1e-06,	-2.13559041180063*1e-06,	-1.80601766808069*1e-06,	-1.88311126723106*1e-06,	-3.07819477112698*1e-06,	-5.17306011268180*1e-06,	-2.96640311185809*1e-06,
			-3.00899690066845*1e-06,	-2.17412546701932*1e-06,	-2.15382034382745*1e-06,	-2.69866949058970*1e-06,	-5.19346481602435*1e-06,	-3.89543552329908*1e-06,	-2.53897441040656*1e-06,
			-3.00899690066845*1e-06,	-2.17412546701932*1e-06,	-2.53897441040656*1e-06,	-3.89543552329908*1e-06,	-5.19346481602435*1e-06,	-2.69866949058970*1e-06,	-2.15382034382745*1e-06,
			-2.70099739689445*1e-06,	-2.13559041180063*1e-06,	-2.96640311185809*1e-06,	-5.17306011268180*1e-06,	-3.07819477112698*1e-06,	-1.88311126723106*1e-06,	-1.80601766808069*1e-06,
			-2.25902290915744*1e-06,	-2.12824503200163*1e-06,	-3.55052980292162*1e-06,	-4.17140909972325*1e-06,	-1.80299627067539*1e-06,	-1.36572542664713*1e-06,	-1.54098776944301*1e-06,
			-1.85652982126111*1e-06,	-2.26120807255429*1e-06,	-4.21360234587828*1e-06,	-2.28602171554116*1e-06,	-1.18036237305197*1e-06,	-1.06609554873689*1e-06,	-1.39382382931709*1e-06,
			-1.57621934794434*1e-06,	-2.69259341848311*1e-06,	-3.46486130426331*1e-06,	-1.40111127004305*1e-06,	-8.84005456272177*1e-07,	-9.23068023384328*1e-07,	-1.39114476121602*1e-06,
			-1.43676746989355*1e-06,	-3.50297447689559*1e-06,	-2.18441630395307*1e-06,	-1.03564689815154*1e-06,	-7.63816088771978*1e-07,	-9.07753536601227*1e-07,	-1.59432882220592*1e-06};

this-> Ipf_corr=(float[7]){0,0,0,0,0,0,0};
this-> Ipf=(float[7]){0,0,0,0,0,0,0};			
*/

//new unina

//START 12 FILAMENTS
// Initialization of the variables for calculating the centroid position for multifilament model
// 12 filaments * 12 coils
// radius 5.5 [cm] from central filament to radial filaments
this-> numFilament = 12.0;

this-> Mpf_SVD=(float[144]){558380.834539853,	-423041.791246495,	263327.797101461,	-117309.938342375,	18816.4612627471,	65143.6637366474,	-147360.361179166,	270329.883501855,	-445570.201224130,	698635.177912398,	-611888.015178159,	-195478.217258221,
						-351320.568694366,	232777.956679177,	-93646.2286901853,	-7185.85477672278,	91746.1258852966,	-163729.744573662,	264323.407369461,	-409465.059257766,	645104.987090318,	-649358.957708526,	-19006.1941582380,	412828.022050860,
						206676.385056415,	-75189.5417181656,	-30685.4988131662,	120793.213499530,	-186961.544921214,	268488.847546706,	-377144.931154907,	564588.447575830,	-580158.339458776,	32947.1963442002,	321316.238799084,	-292980.584837129,
						-86324.9591244337,	-33350.9238238512,	140825.803347829,	-211379.817615473,	282636.314667562,	-359278.218670154,	491319.248511717,	-464582.003935123,	-12026.5258943471,	305457.778106958,	-282383.228035577,	214192.104195711,
						5429.02329895389,	129619.343028226,	-219957.314219746,	294373.117191320,	-349598.010209449,	433289.496594810,	-340148.855366425,	-106464.221031153,	336591.971720720,	-306460.236741382,	251785.836436946,	-135925.991520779,
						75081.6721889952,	-193286.762619824,	285950.211983755,	-334004.662558612,	383346.199296199,	-219653.163919386,	-219653.163919386,	383346.199296199,	-334004.662558611,	285950.211983754,	-193286.762619822,	75081.6721889934,
						-135925.991520780,	251785.836436947,	-306460.236741383,	336591.971720720,	-106464.221031154,	-340148.855366424,	433289.496594810,	-349598.010209447,	294373.117191319,	-219957.314219744,	129619.343028224,	5429.02329895569,
						214192.104195712,	-282383.228035577,	305457.778106959,	-12026.5258943488,	-464582.003935122,	491319.248511716,	-359278.218670153,	282636.314667561,	-211379.817615472,	140825.803347829,	-33350.9238238504,	-86324.9591244341,
						-292980.584837127,	321316.238799083,	32947.1963442008,	-580158.339458776,	564588.447575829,	-377144.931154907,	268488.847546706,	-186961.544921213,	120793.213499529,	-30685.4988131648,	-75189.5417181663,	206676.385056414,
						412828.022050857,	-19006.1941582346,	-649358.957708529,	645104.987090320,	-409465.059257768,	264323.407369461,	-163729.744573662,	91746.1258852962,	-7185.85477672143,	-93646.2286901863,	232777.956679178,	-351320.568694364,
						-195478.217258216,	-611888.015178164,	698635.177912400,	-445570.201224131,	270329.883501855,	-147360.361179165,	65143.6637366468,	18816.4612627479,	-117309.938342376,	263327.797101461,	-423041.791246493,	558380.834539849,
						-436657.366162039,	674730.893462916,	-458337.073904865,	275099.193084250,	-134337.982502335,	42053.1138708534,	42053.1138708538,	-134337.982502335,	275099.193084250,	-458337.073904863,	674730.893462913,	-436657.366162035};

this-> Mfp=(float[144]){-2.45296781962236*1e-06,	-1.58594869690764*1e-06,	-1.12284303843728*1e-06,	-8.69566623232310*1e-07,	-7.46194022751077*1e-07,	-7.22190210737269*1e-07,	-7.96189821590793*1e-07,	-9.97358557123770*1e-07,	-1.40881055613243*1e-06,	-2.24161373678392*1e-06,	-3.78489714571048*1e-06,	-3.92362243561627*1e-06,
						-1.88136839802488*1e-06,	-1.36484238934505*1e-06,	-1.05696547924905*1e-06,	-8.81364104005589*1e-07,	-8.08741240093737*1e-07,	-8.36787691801664*1e-07,	-9.93243029959536*1e-07,	-1.36389482444318*1e-06,	-2.18099250404522*1e-06,	-3.83724114473467*1e-06,	-4.23661659540435*1e-06,	-2.79205928567117*1e-06,
						-1.70844418324814*1e-06,	-1.35686410442625*1e-06,	-1.13478359963620*1e-06,	-1.01623016427194*1e-06,	-1.00187376353235*1e-06,	-1.12156940879505*1e-06,	-1.46601830280168*1e-06,	-2.29119577905016*1e-06,	-4.08915326854542*1e-06,	-4.68997077736363*1e-06,	-3.21802231238139*1e-06,	-2.26514282773094*1e-06,
						-1.73585635155098*1e-06,	-1.49335409451574*1e-06,	-1.34696445892736*1e-06,	-1.30274268706546*1e-06,	-1.39748304090180*1e-06,	-1.73193952973543*1e-06,	-2.58797699243092*1e-06,	-4.53016293822901*1e-06,	-5.20920730706322*1e-06,	-3.64663110736050*1e-06,	-2.66393121909224*1e-06,	-2.09871306380431*1e-06,
						-1.87704230688917*1e-06,	-1.74742686980134*1e-06,	-1.70855862619957*1e-06,	-1.80511112227985*1e-06,	-2.15104699965038*1e-06,	-3.05191073227535*1e-06,	-5.09108742389632*1e-06,	-5.65035253231272*1e-06,	-3.93265257166376*1e-06,	-2.94049780923177*1e-06,	-2.41084647354936*1e-06,	-2.08869205235712*1e-06,
						-2.06516254808807*1e-06,	-2.08809635793831*1e-06,	-2.23321285239164*1e-06,	-2.62166844669619*1e-06,	-3.56754856082123*1e-06,	-5.59731011082052*1e-06,	-5.81776976580696*1e-06,	-3.91687562313287*1e-06,	-2.93804697766332*1e-06,	-2.47859163529721*1e-06,	-2.24338320523949*1e-06,	-2.11835040771061*1e-06,
						-2.24338320523949*1e-06,	-2.47859163529721*1e-06,	-2.93804697766332*1e-06,	-3.91687562313287*1e-06,	-5.81776976580696*1e-06,	-5.59731011082052*1e-06,	-3.56754856082123*1e-06,	-2.62166844669619*1e-06,	-2.23321285239164*1e-06,	-2.08809635793831*1e-06,	-2.06516254808807*1e-06,	-2.11835040771061*1e-06,
						-2.41084647354936*1e-06,	-2.94049780923177*1e-06,	-3.93265257166376*1e-06,	-5.65035253231272*1e-06,	-5.09108742389632*1e-06,	-3.05191073227535*1e-06,	-2.15104699965038*1e-06,	-1.80511112227985*1e-06,	-1.70855862619957*1e-06,	-1.74742686980134*1e-06,	-1.87704230688917*1e-06,	-2.08869205235712*1e-06,
						-2.66393121909224*1e-06,	-3.64663110736050*1e-06,	-5.20920730706322*1e-06,	-4.53016293822901*1e-06,	-2.58797699243092*1e-06,	-1.73193952973543*1e-06,	-1.39748304090180*1e-06,	-1.30274268706546*1e-06,	-1.34696445892736*1e-06,	-1.49335409451574*1e-06,	-1.73585635155098*1e-06,	-2.09871306380431*1e-06,
						-3.21802231238139*1e-06,	-4.68997077736363*1e-06,	-4.08915326854542*1e-06,	-2.29119577905016*1e-06,	-1.46601830280168*1e-06,	-1.12156940879505*1e-06,	-1.00187376353235*1e-06,	-1.01623016427194*1e-06,	-1.13478359963620*1e-06,	-1.35686410442625*1e-06,	-1.70844418324814*1e-06,	-2.26514282773094*1e-06,
						-4.23661659540435*1e-06,	-3.83724114473467*1e-06,	-2.18099250404522*1e-06,	-1.36389482444318*1e-06,	-9.93243029959536*1e-07,	-8.36787691801664*1e-07,	-8.08741240093737*1e-07,	-8.81364104005589*1e-07,	-1.05696547924905*1e-06,	-1.36484238934505*1e-06,	-1.88136839802488*1e-06,	-2.79205928567117*1e-06,
						-3.78489714571048*1e-06,	-2.24161373678392*1e-06,	-1.40881055613243*1e-06,	-9.97358557123770*1e-07,	-7.96189821590793*1e-07,	-7.22190210737269*1e-07,	-7.46194022751077*1e-07,	-8.69566623232310*1e-07,	-1.12284303843728*1e-06,	-1.58594869690764*1e-06,	-2.45296781962236*1e-06,	-3.92362243561627*1e-06};

this-> Ipf_corr=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
this-> Ipf=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};

this->IfilR=(float[12]){50.7631397208144, 48.7500000000000, 46, 43.2500000000000, 41.2368602791856, 40.5000000000000, 41.2368602791856, 43.2500000000000, 46, 48.7500000000000, 50.7631397208144, 51.5000000000000};//is in [cm]
this->IfilZ=(float[12]){2.75000000000000, 4.76313972081441, 5.50000000000000, 4.76313972081441, 2.75000000000000, 0, -2.75000000000000, -4.76313972081441, -5.50000000000000, -4.76313972081441, -2.75000000000000, 0};//is in [cm]


//END 12 FILAMENTS

/*
//START 15 FILAMENTS
// Initialization of the variables for calculating the centroid position for multifilament model
// 15 filaments * 12 coils
// radius 5.5 [cm] from central filament to radial filaments

this-> numFilament = 15.0;
this-> Mpf_SVD=(float[180]){-257767.794676788,	331235.210568796,	-157218.122641702,	71673.6841501456,	-26286.7130223886,	8472.93918218441,	8472.93918218721,	-26286.7130223913,	71673.6841501478,	-157218.122641704,	331235.210568795,	-257767.794676787,
								661082.278047142,	-327287.618037194,	150235.792917623,	-60361.8710217007,	27136.2933417948,	-8088.13688628755,	240.782693721243,	13855.9216293408,	-26422.0629916075,	44213.5394526034,	189175.784828871,	-718972.635022676,
								-404582.130953307,	200301.349266648,	-85244.8756337189,	41381.0997568709,	-20075.6711192299,	18211.8814028479,	-21390.0617958772,	47632.4318852106,	-112516.703343837,	411887.551170392,	-900992.431468040,	781440.091390544,
								154643.618279177,	-71538.2156501851,	40713.4360140430,	-25159.3695199270,	30347.1745724331,	-43229.0552724276,	87580.4540961803,	-183542.122271629,	464811.282443686,	-820184.875457400,	625512.299688165,	-291154.524418807,
								-7109.32316637494,	14996.1384264667,	-17235.7397333041,	32782.3081617495,	-50571.4466561992,	96162.0285476127,	-183555.451472787,	397242.184168053,	-585854.530356064,	317789.268609321,	-56720.4396110401,	21938.4834162785,
								-36097.6982158973,	10243.1035000827,	17149.9161069384,	-35817.0614727885,	70981.3728557926,	-121397.718646821,	231842.874976746,	-245454.589897211,	-64038.1450232018,	213586.939975391,	-139790.857334210,	86830.1531904395,
								45902.7951617241,	-12808.6840495917,	-1139.62171959662,	20157.7900246381,	-29457.6804893324,	45016.0892919417,	80547.3107418420,	-390135.449418604,	416984.385466775,	-256393.807500140,	158884.681093555,	-84379.4517127140,
								-36467.0664881526,	37221.8334371331,	-40637.6743964004,	70569.4142906460,	-119306.848046947,	306015.340012068,	-538676.444311693,	437065.238554201,	-243598.946380707,	151308.711149836,	-86429.0610335618,	58521.8868938961,
								58521.8868938935,	-86429.0610335604,	151308.711149836,	-243598.946380708,	437065.238554202,	-538676.444311695,	306015.340012069,	-119306.848046949,	70569.4142906481,	-40637.6743964020,	37221.8334371335,	-36467.0664881515,
								-84379.4517127116,	158884.681093554,	-256393.807500140,	416984.385466777,	-390135.449418606,	80547.3107418440,	45016.0892919394,	-29457.6804893296,	20157.7900246355,	-1139.62171959494,	-12808.6840495919,	45902.7951617226,
								86830.1531904409,	-139790.857334211,	213586.939975391,	-64038.1450232005,	-245454.589897213,	231842.874976747,	-121397.718646822,	70981.3728557938,	-35817.0614727893,	17149.9161069384,	10243.1035000834,	-36097.6982158986,
								21938.4834162748,	-56720.4396110384,	317789.268609321,	-585854.530356066,	397242.184168056,	-183555.451472790,	96162.0285476155,	-50571.4466562023,	32782.3081617523,	-17235.7397333056,	14996.1384264663,	-7109.32316637263,
								-291154.524418806,	625512.299688164,	-820184.875457398,	464811.282443685,	-183542.122271628,	87580.4540961794,	-43229.0552724269,	30347.1745724326,	-25159.3695199268,	40713.4360140434,	-71538.2156501862,	154643.618279179,
								781440.091390546,	-900992.431468040,	411887.551170389,	-112516.703343834,	47632.4318852069,	-21390.0617958734,	18211.8814028443,	-20075.6711192261,	41381.0997568671,	-85244.8756337175,	200301.349266649,	-404582.130953310,
								-718972.635022676,	189175.784828871,	44213.5394526037,	-26422.0629916076,	13855.9216293412,	240.782693720692,	-8088.13688628726,	27136.2933417941,	-60361.8710216997,	150235.792917623,	-327287.618037195,	661082.278047143};

this-> Mfp=(float[180]){-3.92362243561627*1e-06,	-2.70441119131170*1e-06,	-1.86713355355569*1e-06,	-1.36687376879987*1e-06,	-1.05900518047176*1e-06,	-8.69566623232310*1e-07,	-7.62409951988404*1e-07,	-7.20546810264455*1e-07,	-7.39197838501142*1e-07,	-8.24624453143891*1e-07,	-9.97358557123770*1e-07,	-1.30243200909618*1e-06,	-1.83508890736982*1e-06,	-2.78035117562343*1e-06,	-4.07451127681704*1e-06,
							-2.79205928567117*1e-06,	-2.02304718800848*1e-06,	-1.53947465541807*1e-06,	-1.22245177447992*1e-06,	-1.01266224238087*1e-06,	-8.81364104005589*1e-07,	-8.15496406139269*1e-07,	-8.12556428796516*1e-07,	-8.80987830547261*1e-07,	-1.04581395104430*1e-06,	-1.36389482444318*1e-06,	-1.96136300799392*1e-06,	-3.07897573337920*1e-06,	-4.42751562315242*1e-06,	-3.95124141408664*1e-06,
							-2.26514282773094*1e-06,	-1.79952160296769*1e-06,	-1.47929063614581*1e-06,	-1.25466454676888*1e-06,	-1.10310581447605*1e-06,	-1.01623016427194*1e-06,	-9.95532052224262*1e-07,	-1.05385185606478*1e-06,	-1.22364469373498*1e-06,	-1.57921180487437*1e-06,	-2.29119577905016*1e-06,	-3.65823532003772*1e-06,	-4.92871026459921*1e-06,	-4.09051967155174*1e-06,	-2.98072672503282*1e-06,
							-2.09871306380431*1e-06,	-1.79730474898243*1e-06,	-1.57828168039130*1e-06,	-1.42349900539690*1e-06,	-1.32929645975385*1e-06,	-1.30274268706546*1e-06,	-1.36404693145106*1e-06,	-1.55784135598430*1e-06,	-1.98440568448439*1e-06,	-2.87415660712017*1e-06,	-4.53016293822901*1e-06,	-5.41787845355257*1e-06,	-4.23404167042924*1e-06,	-3.18107105095090*1e-06,	-2.52719723020990*1e-06,
							-2.08869205235712*1e-06,	-1.91252500346018*1e-06,	-1.78957453300384*1e-06,	-1.71943617216741*1e-06,	-1.71477346919170*1e-06,	-1.80511112227985*1e-06,	-2.05183219763085*1e-06,	-2.58974571604884*1e-06,	-3.71330030794690*1e-06,	-5.54282171410149*1e-06,	-5.65035253231272*1e-06,	-4.22595293844604*1e-06,	-3.25901003143164*1e-06,	-2.69258343047273*1e-06,	-2.33438668720525*1e-06,
							-2.11835040771061*1e-06,	-2.07038000720981*1e-06,	-2.06794739136040*1e-06,	-2.12705654084785*1e-06,	-2.28538652977080*1e-06,	-2.62166844669619*1e-06,	-3.30616630762197*1e-06,	-4.66311080944160*1e-06,	-6.27892508793777*1e-06,	-5.40149227502737*1e-06,	-3.91687562313287*1e-06,	-3.07751923178276*1e-06,	-2.62482839566195*1e-06,	-2.36641355208404*1e-06,	-2.21154048863337*1e-06,
							-2.11835040771061*1e-06,	-2.21154048863337*1e-06,	-2.36641355208404*1e-06,	-2.62482839566195*1e-06,	-3.07751923178276*1e-06,	-3.91687562313287*1e-06,	-5.40149227502737*1e-06,	-6.27892508793777*1e-06,	-4.66311080944160*1e-06,	-3.30616630762197*1e-06,	-2.62166844669619*1e-06,	-2.28538652977080*1e-06,	-2.12705654084785*1e-06,	-2.06794739136040*1e-06,	-2.07038000720981*1e-06,
							-2.08869205235712*1e-06,	-2.33438668720525*1e-06,	-2.69258343047273*1e-06,	-3.25901003143164*1e-06,	-4.22595293844604*1e-06,	-5.65035253231272*1e-06,	-5.54282171410149*1e-06,	-3.71330030794690*1e-06,	-2.58974571604884*1e-06,	-2.05183219763085*1e-06,	-1.80511112227985*1e-06,	-1.71477346919170*1e-06,	-1.71943617216741*1e-06,	-1.78957453300384*1e-06,	-1.91252500346018*1e-06,
							-2.09871306380431*1e-06,	-2.52719723020990*1e-06,	-3.18107105095090*1e-06,	-4.23404167042924*1e-06,	-5.41787845355257*1e-06,	-4.53016293822901*1e-06,	-2.87415660712017*1e-06,	-1.98440568448439*1e-06,	-1.55784135598430*1e-06,	-1.36404693145106*1e-06,	-1.30274268706546*1e-06,	-1.32929645975385*1e-06,	-1.42349900539690*1e-06,	-1.57828168039130*1e-06,	-1.79730474898243*1e-06,
							-2.26514282773094*1e-06,	-2.98072672503282*1e-06,	-4.09051967155174*1e-06,	-4.92871026459921*1e-06,	-3.65823532003772*1e-06,	-2.29119577905016*1e-06,	-1.57921180487437*1e-06,	-1.22364469373498*1e-06,	-1.05385185606478*1e-06,	-9.95532052224262*1e-07,	-1.01623016427194*1e-06,	-1.10310581447605*1e-06,	-1.25466454676888*1e-06,	-1.47929063614581*1e-06,	-1.79952160296769*1e-06,
							-2.79205928567117*1e-06,	-3.95124141408664*1e-06,	-4.42751562315242*1e-06,	-3.07897573337920*1e-06,	-1.96136300799392*1e-06,	-1.36389482444318*1e-06,	-1.04581395104430*1e-06,	-8.80987830547261*1e-07,	-8.12556428796516*1e-07,	-8.15496406139269*1e-07,	-8.81364104005589*1e-07,	-1.01266224238087*1e-06,	-1.22245177447992*1e-06,	-1.53947465541807*1e-06,	-2.02304718800848*1e-06,
							-3.92362243561627*1e-06,	-4.07451127681704*1e-06,	-2.78035117562343*1e-06,	-1.83508890736982*1e-06,	-1.30243200909618*1e-06,	-9.97358557123770*1e-07,	-8.24624453143891*1e-07,	-7.39197838501142*1e-07,	-7.20546810264455*1e-07,	-7.62409951988404*1e-07,	-8.69566623232310*1e-07,	-1.05900518047176*1e-06,	-1.36687376879987*1e-06,	-1.86713355355569*1e-06,	-2.70441119131170*1e-06 };


							
this->IfilR=(float[15]){51.5000000000000,	51.0245000170343,	49.6802183349737,	47.6995934690622,	45.4250934520279,	43.2500000000000,	41.5504065309378,	40.6201881959641,	40.6201881959641,	41.5504065309378,	43.2500000000000,	45.4250934520279,	47.6995934690622,	49.6802183349737,	51.0245000170343};							
this->IfilZ=(float[15]){0,					2.23705153691690,	4.08729654012567,	5.23081083962334,	5.46987042452550,	4.76313972081441,	3.23281888760860,	1.14351429949768,	-1.14351429949768,	-3.23281888760860,	-4.76313972081441,	-5.46987042452550,	-5.23081083962334,	-4.08729654012567,	-2.23705153691690};

this-> Ipf_corr=(float[15]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
this-> Ipf=(float[15]){0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

//END 15 FILAMENTS

*/
//end
this-> Bmag_rec=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
this-> Bmag_rec_corr=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
	
	
					
					
					
					
					
AssertErrorCondition(Information,"MagneticsGAM:: GAMOffline: C_hor[0][0]: %f ", (float)ADC_fact[0]);

				

// Initialization of outputs adn other signals
//	this ->y_prim=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
//	this ->y_vert=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
//	this ->y_hor=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
	this->ADC_ext_flux=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
	this->ADC_final=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
	this->buffer_hor = 0.0;
	this->y_buff2 = 0.0;
	
		this->y_vert= new float[this->NumberOfProbes];
		this->y_prim= new float[this->NumberOfProbes];
		this->y_hor= new float[this->NumberOfProbes];
		
for (i = 0; i < this->NumberOfProbes; i++) {
		this->y_vert[i] = 0.0;
		this->y_hor[i] = 0.0;
		this->y_prim[i] = 0.0;}

//////////////////////////////////////////////////////////////////////////////////
	this->radial_coeficients = new float[this->NumberOfProbes];
	this->vertical_coeficients = new float[this->NumberOfProbes];
	for (i = 0; i < this->NumberOfProbes; i++) {

		this->radial_coeficients[i] = this->probe_radius * cos(this->magnetic_Angles[i] * M_PI / 180);
		this->vertical_coeficients[i] = this->probe_radius * sin(this->magnetic_Angles[i] * M_PI / 180);
	}

	if (NumberOfMeasurements == NumberOfProbes) {

		this->points_x = new float[NumberOfProbes / 4];
		this->points_y = new float[NumberOfProbes / 4];
		this->m_x = new float[NumberOfProbes];
		this->m_y = new float[NumberOfProbes];

		for (i = 0; i < this->NumberOfProbes; i++) {
			m_x[i] = this->radial_coeficients[i] / this->probe_radius;
			m_y[i] = this->vertical_coeficients[i] / this->probe_radius;
		}
	}

	//this->plasma_current_convertion_factor = 4300 * 2.0 * M_PI * this->probe_radius / this->NumberOfMeasurements;
	this->plasma_current_convertion_factor = -1.0*(2.0 * M_PI * this->probe_radius / this->Ncoils)*(1/(this->MAgPerm*this->Nvoltas*this->Area));
	this->saved_usectime = 0;
	this->lastmirnov = new float[this->NumberOfProbes];
	for(i=0;i<this->NumberOfProbes;i++){
		this->lastmirnov[i] = (float) 0;
	}	
	this->mirnovaccumulator = new float[this->NumberOfProbes];
	for(i=0;i<this->NumberOfProbes;i++){
		this->mirnovaccumulator[i] = (float) 0;
	}	
	this->accumulatorcounter = 0;
	this->k = 0;
	this->m = 0;
	this->n = 0;
	this->o = 0;
	this->p = 0;
	this->buff = 0.0;
	int doonce = 1;
	
	return True;
}
//} ******* End of bool MagneticsGAM::Initialise(ConfigurationDataBase& cdbData)*************************


//{ ********* Execute the module functionalities *******************
bool MagneticsGAM::Execute(GAM_FunctionNumbers functionNumber) {

	InputInterfaceStruct *inputstruct = (InputInterfaceStruct *) this->SignalsInputInterface->Buffer();
	this->SignalsInputInterface->Read();
	OutputInterfaceStruct *outputstruct = (OutputInterfaceStruct *) this->SignalsOutputInterface->Buffer();

	
	
	

	ADC_values[0] = (float)inputstruct[0].ADC_magnetic_chopper_fp_0;
	ADC_values[1] = (float)inputstruct[0].ADC_magnetic_chopper_fp_1;
	ADC_values[2] = (float)inputstruct[0].ADC_magnetic_chopper_fp_2;
	ADC_values[3] = (float)inputstruct[0].ADC_magnetic_chopper_fp_3;
	ADC_values[4] = (float)inputstruct[0].ADC_magnetic_chopper_fp_4;
	ADC_values[5] = (float)inputstruct[0].ADC_magnetic_chopper_fp_5;
	ADC_values[6] = (float)inputstruct[0].ADC_magnetic_chopper_fp_6;
	ADC_values[7] = (float)inputstruct[0].ADC_magnetic_chopper_fp_7;
	ADC_values[8] = (float)inputstruct[0].ADC_magnetic_chopper_fp_8;
	ADC_values[9] = (float)inputstruct[0].ADC_magnetic_chopper_fp_9;
	ADC_values[10] = (float)inputstruct[0].ADC_magnetic_chopper_fp_10;
	ADC_values[11] = (float)inputstruct[0].ADC_magnetic_chopper_fp_11;

	// Measured horizontal, Vertical & Primary currents
	prim_meas= inputstruct[0].PrimaryCurrent;
	hor_meas=  inputstruct[0].HorizontalCurrent;
	vert_meas= inputstruct[0].VerticalCurrent;
	
	

	//Apply coil polarity factor - OK ---vamos usar depois, pra calcular posicao e corrente 
	//for (i = 0; i < this->NumberOfMeasurements; i++) {
		//ADC_values[i] = ADC_values[i] * magnetic_Polarity_calibration[i];
	//}
	


	//if(functionNumber == GAMOffline){
		//if (((uint)inputstruct[0].usectime - this->saved_usectime) > 30000000 || ((uint)inputstruct[0].usectime - this->saved_usectime) < 0 ) {
			//AssertErrorCondition(Information,"MagneticsGAM:: GAMOffline: usectime: %i ", (uint)inputstruct[0].usectime);
			//slope = ((this->ADC_values[0] - this->lastmirnov[0])) / ((float)((uint)inputstruct[0].usectime - this->saved_usectime + 0.1));
			//AssertErrorCondition(Information,"MagneticsGAM:: GAMOffline: usectime: %i, ADC_magnetic_chopper_fp_0 : %i, ADC: %f, slope: %f", (uint)inputstruct[0].usectime,inputstruct[0].ADC_magnetic_chopper_fp_0, ADC_values[0],slope);
			//this->saved_usectime = (uint)inputstruct[0].usectime;
			//lastmirnov[0] = ADC_values[0];
		//}
		//this->lasttime=inputstruct[0].usectime;
		//this->k=0;
	//}




	if (functionNumber == GAMOnline) {
		// Determine the ADC Module offset "b" as "y(n)=a*n+b"
		if (((uint)inputstruct[0].usectime - this->lasttime) > 89000000) { // Start averaging 1s before start taking slope averages
			this->accumulatorcounter++ ;
			for(i=0;i<this->NumberOfProbes;i++){
				this->mirnovaccumulator[i] = (mirnovaccumulator[i] * (accumulatorcounter - 1) + ADC_values[i])/accumulatorcounter;
			}	
			
		}
		
		if (((uint)inputstruct[0].usectime - this->saved_usectime) > 999900 || ((uint)inputstruct[0].usectime - this->saved_usectime) < 0 ) { //compara um segundo
			
			slope = (this->mirnovaccumulator[0] - this->lastmirnov[0]) / ((float)((uint)inputstruct[0].usectime - this->saved_usectime + 1));

			//AssertErrorCondition(Information,"MagneticsGAM:: GAMOnline: usectime: %i, ADC_magnetic_chopper_fp_0 : %f, slope: %f", (uint)inputstruct[0].usectime, ADC_values[0],slope);
									
			if (((uint)inputstruct[0].usectime - this->lasttime) > 90000000 && ((uint)inputstruct[0].usectime - this->lasttime) < 120000000) {
				//AssertErrorCondition(Information, "MagneticsGAM:: GAMOnline: sloping it");

				for (i = 0; i < this->NumberOfMeasurements; i++) {
					
					//this->slopes[i][k] = ((this->ADC_values[i] - this->lastmirnov[i])) / ((float)((uint)inputstruct[0].usectime - this->saved_usectime));
					this->slopes[i][p] = ((this->mirnovaccumulator[i] - this->lastmirnov[i])) / ((float)((uint)inputstruct[0].usectime - this->saved_usectime + 1));
				}
				p++;
				
				
				// WO's average

				for (j = 0; j < this->NumberOfMeasurements; j++) {
					for (i = 0; i < 29; i++) {
						this->buff = this->buff + this->slopes[j][i];
					}
					this -> slope_avrg[j] = this->buff / (29) ;
					this->buff = 0.0;
				}
				
		
			}
			else p=0;
			

			//for (i = 0; i < this->NumberOfMeasurements; i++) {
			//	this->lastmirnov[i] = this->ADC_values[i];
			//}
			for (i = 0; i < NumberOfMeasurements; i++) {
				lastmirnov[i] = mirnovaccumulator[i];
				mirnovaccumulator[i] = (float) 0;
			}			
			this->accumulatorcounter = 0;
			
			this->saved_usectime = (uint)inputstruct[0].usectime;
		}

		

		if (inputstruct[0].usectime > 0 && inputstruct[0].usectime < usectime_to_wait_for_starting_operation) {

			//For now we do not use this step (under optimization)

			//Determine "b" by knowing "a" and "y(-100us)"
			//if(inputstruct[0].usectime==900){
			//	for(i = 0 ; i < this->NumberOfMeasurements ; i++){
			//		this->magnetic_Offset_zero[i] = ADC_values[i] + this->magnetic_Offset_slope[i]; // b = y(-100us) - a*(-100us) = y(10) + a*(1)
			//	}				
			//}			
			
	

			outputstruct[0].MagneticProbesR = 0.;
			outputstruct[0].MagneticProbesZ = 0.;
			outputstruct[0].MagneticProbesPlasmaCurrent = 0.;
			outputstruct[0].Magnetics_R_uncorrctd= 0.;
			outputstruct[0].Magnetics_z_uncorrctd = 0.;
			outputstruct[0].Magnetics_Ip_corrctd = 0.;
			
			i=(int)inputstruct[0].usectime/100;
			if (i==1||i==2||i==3){
				outputstruct[0].ADC_magnetic_WO_corrctd_0 =  slope_avrg[0]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_1 =  slope_avrg[1]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_2 =  slope_avrg[2]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_3 =  slope_avrg[3]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_4 = slope_avrg[4]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_5 = slope_avrg[5]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_6 = slope_avrg[6]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_7 = slope_avrg[7]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_8 = slope_avrg[8]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_9 = slope_avrg[9]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_10 = slope_avrg[10]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_11 = slope_avrg[11]*1e-10;
			}
			else {
			if (i>28) {i=28;}
			// esTamos a multiplicar arbitrariamente por 1e-10 para reduzir a ordem de grandeza do sinal
			outputstruct[0].ADC_magnetic_WO_corrctd_0 = slopes[0][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_1 = slopes[1][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_2 = slopes[2][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_3 = slopes[3][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_4 = slopes[4][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_5 = slopes[5][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_6 = slopes[6][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_7 = slopes[7][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_8 = slopes[8][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_9 = slopes[9][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_10 = slopes[10][i]*1e-10;
			outputstruct[0].ADC_magnetic_WO_corrctd_11 = slopes[11][i]*1e-10;
			}
			
			
		} // ********* End usectime < usectime_to_wait_for_starting_operation *******
		else {

			//Take offset at t=0
			if (inputstruct[0].usectime == usectime_to_wait_for_starting_operation) {
				for (i = 0; i < this->NumberOfMeasurements; i++) {
					this->magnetic_Offset_zero[i] = ADC_values[i];
					magnetic_field_sum = 0.0;
					magnetic_field_sum_corr = 0.0;
				}
			}

			//Correct using corrected= ADC[n]-(m*x+b), tirei o 1/100
			for (i = 0; i < this->NumberOfMeasurements; i++) {
				corrected_probes[i] = ADC_values[i] - (this->magnetic_Offset_slope[i] * ((inputstruct[0].usectime - usectime_to_wait_for_starting_operation) ) + this->magnetic_Offset_zero[i]);
			}

			//Correct usign the saved averaged slopes

			for (i = 0; i < this->NumberOfMeasurements; i++) {
				ADC_WO[i] = ADC_values[i] - this->slope_avrg[i] * ((inputstruct[0].usectime - usectime_to_wait_for_starting_operation)) - this->magnetic_Offset_zero[i];
			}
			
			// [LSB x sample] -> [V x s] or [Wb]    e com polaridade corregida

			for (i = 0; i < this->NumberOfMeasurements; i++) {
				ADC_WO_Wb[i]=ADC_fact[i]*magnetic_Polarity_calibration[i]*ADC_WO[i];
				}


			//Write the value of the 12 mirnov channel without the WO in Webers and polarity factor corrected
			outputstruct[0].ADC_magnetic_WO_corrctd_0 = ADC_WO_Wb[0];
			outputstruct[0].ADC_magnetic_WO_corrctd_1 = ADC_WO_Wb[1];
			outputstruct[0].ADC_magnetic_WO_corrctd_2 = ADC_WO_Wb[2];
			outputstruct[0].ADC_magnetic_WO_corrctd_3 = ADC_WO_Wb[3];
			outputstruct[0].ADC_magnetic_WO_corrctd_4 = ADC_WO_Wb[4];
			outputstruct[0].ADC_magnetic_WO_corrctd_5 = ADC_WO_Wb[5];
			outputstruct[0].ADC_magnetic_WO_corrctd_6 = ADC_WO_Wb[6];
			outputstruct[0].ADC_magnetic_WO_corrctd_7 = ADC_WO_Wb[7];
			outputstruct[0].ADC_magnetic_WO_corrctd_8 = ADC_WO_Wb[8];
			outputstruct[0].ADC_magnetic_WO_corrctd_9 = ADC_WO_Wb[9];
			outputstruct[0].ADC_magnetic_WO_corrctd_10 = ADC_WO_Wb[10];
			outputstruct[0].ADC_magnetic_WO_corrctd_11 = ADC_WO_Wb[11];
			


			//Compute fluxes to be substracted (Discrete State-Space equations)
			
				
				for (i = 0; i < this->NumberOfMeasurements; i++) {
					this ->y_prim[i]=this ->C_prim[i]* this ->x_prim[i];					
					this ->x_prim[i]=this ->A_prim[i]* this ->x_prim[i]+ (this ->B_prim[i]*prim_meas);
					
					this ->y_vert[i]=this ->C_vert[i]*this ->x_vert[i];
					this ->x_vert[i]=this ->A_vert[i]*this ->x_vert[i]+(this ->B_vert[i]*vert_meas);					
					this -> y_hor[i]=0.0;
				}
				
				
				
				for (i = 0; i < this->NumberOfMeasurements; i++) {
						for (j=0; j< 4; j++){								
								this->m= 4*(i)+j;								
								this -> y_hor[i] += this -> C_hor_vec[m]*this-> x_hor_vec[m];																									
																				}										
																				}
						
											
			
					
			for (i = 0; i < NumberOfMeasurements; i++) {
					for (j=0; j< 4; j++){						
						m= 4*(i)+j;					
						for (k=0; k< 4; k++){	
										this->n= k+(4*j)+(16*i);
										this->o=4*i+k;									
										this->buffer_hor+=this->A_hor_vec[n]*this->x_hor_vec[o];									
										}
					this->x_buff_hor[m]=this->buffer_hor;
					this->x_buff_hor[m]=this->x_buff_hor[m]+(this->B_hor_vec[m]*hor_meas);		
					this->buffer_hor=0.0;			
								}
								}	
					
					
								
			for (j=0; j< 48; j++){									
								this->x_hor_vec[j]=this->x_buff_hor[j];
										}
																
					

			
			//Substract calculated external fluxes from the mirnov measurements
			
			for (i = 0; i < this->NumberOfMeasurements; i++) {
				ADC_ext_flux[i]=y_hor[i]+y_prim[i]+y_vert[i];
				ADC_final[i]=ADC_WO_Wb[i]-ADC_ext_flux[i];
				}
				
				/*for (i = 0; i < this->NumberOfMeasurements; i++) {
				ADC_ext_flux[i]=y_vert[i];		
				}*/
				
				
			
			ADC_final[9] = ADC_final[9] * 1.2803;//correction mirnov10			
			//Write the value of the 12 mirnov external fluxes reconstructed
			outputstruct[0].Magnetics_ext_flux_0 = ADC_ext_flux[0];
			outputstruct[0].Magnetics_ext_flux_1 = ADC_ext_flux[1];
			outputstruct[0].Magnetics_ext_flux_2 = ADC_ext_flux[2];
			outputstruct[0].Magnetics_ext_flux_3 = ADC_ext_flux[3];
			outputstruct[0].Magnetics_ext_flux_4 = ADC_ext_flux[4];
			outputstruct[0].Magnetics_ext_flux_5 = ADC_ext_flux[5];
			outputstruct[0].Magnetics_ext_flux_6 = ADC_ext_flux[6];
			outputstruct[0].Magnetics_ext_flux_7 = ADC_ext_flux[7];
			outputstruct[0].Magnetics_ext_flux_8 = ADC_ext_flux[8];
			outputstruct[0].Magnetics_ext_flux_9 = ADC_ext_flux[9];
			outputstruct[0].Magnetics_ext_flux_10 = ADC_ext_flux[10];
			outputstruct[0].Magnetics_ext_flux_11 = ADC_ext_flux[11];
			
			
			//Write the value of the 12 mirnov with flux correction
			outputstruct[0].Magnetics_flux_corrctd_0 = ADC_final[0];
			outputstruct[0].Magnetics_flux_corrctd_1 = ADC_final[1];
			outputstruct[0].Magnetics_flux_corrctd_2 = ADC_final[2];
			outputstruct[0].Magnetics_flux_corrctd_3 = ADC_final[3];
			outputstruct[0].Magnetics_flux_corrctd_4 = ADC_final[4];
			outputstruct[0].Magnetics_flux_corrctd_5 = ADC_final[5];
			outputstruct[0].Magnetics_flux_corrctd_6 = ADC_final[6];
			outputstruct[0].Magnetics_flux_corrctd_7 = ADC_final[7];
			outputstruct[0].Magnetics_flux_corrctd_8 = ADC_final[8];
			outputstruct[0].Magnetics_flux_corrctd_9 = ADC_final[9];
			outputstruct[0].Magnetics_flux_corrctd_10 = ADC_final[10];
			outputstruct[0].Magnetics_flux_corrctd_11 = ADC_final[11];


			// Calculate Ip
			magnetic_field_sum = 0.0;  //this->NumberOfMeasurements
			magnetic_field_sum_corr = 0.0;
			this->PlasmaCurrent = 0.0;//new
			for (i = 0; i < this->NumberOfMeasurements; i++) {
				magnetic_field_sum = ADC_WO_Wb[i]+ magnetic_field_sum;
				magnetic_field_sum_corr = ADC_final[i]+ magnetic_field_sum_corr;
			}
			
			outputstruct[0].MagneticProbesPlasmaCurrent = magnetic_field_sum*this->plasma_current_convertion_factor;//corrected_probes[11]; 
			outputstruct[0].Magnetics_Ip_corrctd = magnetic_field_sum_corr*this->plasma_current_convertion_factor;
			
			this->PlasmaCurrent =  magnetic_field_sum*this->plasma_current_convertion_factor;
																			
			// Estimate radial_position and vertical_position
			radial_position_corr = 0.0;
			vertical_position_corr = 0.0;
			radial_position = 0.0;
			vertical_position = 0.0;
			/* This was done when the integrators were analogic
			if(NumberOfMeasurements == NumberOfProbes){
			// WARNING: this code divides by zero fairly often. Many IGBTs died to bring us this information
			for (i = 0 ; i < this->NumberOfProbes/4  ; i++){
			r_a = this->probe_radius * 2 * corrected_probes[int(i+NumberOfProbes/2)] / (corrected_probes[int(i+NumberOfProbes/2)] + corrected_probes[i]);
			r_b = this->probe_radius * 2 * corrected_probes[int(i+NumberOfProbes/2+NumberOfProbes/4)] / (corrected_probes[int(i+NumberOfProbes/2+NumberOfProbes/4)] + corrected_probes[int(i+NumberOfProbes/4)]);
			x_a = this->radial_coeficients[this->ProbeNumbers[i]] - m_x[i] * r_a;
			x_b = this->radial_coeficients[this->ProbeNumbers[int(i+this->NumberOfProbes/4)]] - m_x[int(i+this->NumberOfProbes/4)] * r_b;
			y_a = this->vertical_coeficients[this->ProbeNumbers[i]] - m_y[i] * r_a;
			y_b = this->vertical_coeficients[this->ProbeNumbers[int(i+this->NumberOfProbes/4)]] - m_y[int(i+this->NumberOfProbes/4)] * r_b;
			//					if (m_x[i] != 0 && m_x[int(i+NumberOfProbes/4)] != 0) {
			m_b = m_y[i]/m_x[i];
			m_a = m_y[int(i+this->NumberOfProbes/4)]/m_x[int(i+this->NumberOfProbes/4)];
			points_x[i] = (m_b*x_b-y_b-m_a*x_a+y_a) / (m_b-m_a);
			points_y[i] = m_a*(points_x[i]-x_a)+y_a;
			//					}
			radial_position += points_x[i];
			vertical_position += points_y[i];
			}

			}
			else {}*/


 /*		OLD CENTROID CALCULATION
 		for(i = 0 ; i < this->NumberOfMeasurements ; i++){

		//	radial_position += ADC_WO_Wb[i] * this->radial_coeficients[this->ProbeNumbers[i]];
		//	vertical_position += ADC_WO_Wb[i] * this->vertical_coeficients[this->ProbeNumbers[i]];
			radial_position += ADC_final[i] * this->radial_coeficients[this->ProbeNumbers[i]];
			vertical_position += ADC_final[i] * this->vertical_coeficients[this->ProbeNumbers[i]];
			}
                      if (magnetic_field_sum !=0) {
			radial_position = radial_position / magnetic_field_sum;
			vertical_position = vertical_position / magnetic_field_sum;
			}
						
			else {
			radial_position = 0;
			vertical_position = 0;
			radial_position_corr = 0.0;
			vertical_position_corr = 0.0;
			}
			
			
			*/
			
			
			//start with multifilament
			
			
			sum_Ifil = 0.0;
			sum_Ifil_corr = 0.0;
			Ipf_buff=0.0;
			Ipf_corr_buff=0.0;
			this->m = 0;
			this->division = 0;
			this->division_corr = 0;

			/* old
			for(i=0; i<7; i++){
				for(j=0; j<12; j++){
					this->m= 12*(i)+j;
					this->Ipf_buff +=  this->Mpf_SVD[m] * (this->ADC_WO_Wb[j]/(50*49*1e-6));
					this->Ipf_corr_buff += this->Mpf_SVD[m] * (this->ADC_final[j]/(50*49*1e-6));
					
				}
				this->Ipf[i] =this->Ipf_buff;
				this->Ipf_corr[i] =this->Ipf_corr_buff;
				this->Ipf_buff=0.0;
				this->Ipf_corr_buff=0.0;
				
			}*/
			
			//NEW
			for(i=0; i<this->numFilament; i++){
				for(j=0; j<12; j++){
					this->m= 12*(i)+j;
					this->Ipf_buff +=  this->Mpf_SVD[m] * (this->ADC_WO_Wb[j]/(50*49*1e-6));
					this->Ipf_corr_buff += this->Mpf_SVD[m] * (this->ADC_final[j]/(50*49*1e-6));
					
				}
				this->Ipf[i] =this->Ipf_buff;
				this->Ipf_corr[i] =this->Ipf_corr_buff;
				this->Ipf_buff=0.0;
				this->Ipf_corr_buff=0.0;
				
			}
			//END
			
			//this->Ipf[0] =this->Mpf_SVD[0] * (this->ADC_WO_Wb[0]/(50*49*1e-6))+ this->Mpf_SVD[1] * (this->ADC_WO_Wb[1]/(50*49*1e-6));
			/*old
			for(i=0; i<7; i++){
				this->sum_Ifil += this->Ipf[i];
				this->sum_Ifil_corr += this->Ipf_corr[i];
				
				this->radial_position_corr += this->Ipf_corr[i] * (this->IfilR[i]*this->IfilR[i]);
				this->radial_position += this->Ipf[i] * (this->IfilR[i]*this->IfilR[i]);
				
				this->vertical_position_corr += this->Ipf_corr[i] * (this->IfilZ[i]);
				this->vertical_position += this->Ipf[i] * (this->IfilZ[i]);
			}*/
			

						
			for(i=0; i<this->numFilament; i++){
				this->sum_Ifil += this->Ipf[i];
				this->sum_Ifil_corr += this->Ipf_corr[i];
				
				this->radial_position_corr += this->Ipf_corr[i] * (this->IfilR[i]*this->IfilR[i]);
				this->radial_position += this->Ipf[i] * (this->IfilR[i]*this->IfilR[i]);
				
				this->vertical_position_corr += this->Ipf_corr[i] * (this->IfilZ[i]);
				this->vertical_position += this->Ipf[i] * (this->IfilZ[i]);
			}
		
			// Let's put the measurements in [m] and put the origin of the centroid in the center of the chamber
			this->division_corr = this->radial_position_corr / this->sum_Ifil_corr; 
			this->division = this->radial_position / this->sum_Ifil;
			
			if(this->PlasmaCurrent>750 || this->PlasmaCurrent<-750) {			
				if(sum_Ifil!=0){
					this->vertical_position = 0.01*(this->vertical_position / this->sum_Ifil);
					if(this->division<0){//a negative number to a positive number, too hard?
						this->radial_position = this->clip_limit;
					}
					else{
					//this->radial_position = (0.01*sqrt(this->division_corr))-0.46;
						this->radial_position = (0.01*sqrt(this->division))-0.46;
					}
				}
			
				else{
					this->radial_position = this->clip_limit;
					this->vertical_position =this->clip_limit;
				}
						
			//// Also for the corrected position (just in case)
				if(sum_Ifil_corr!=0){
					this->vertical_position_corr =  0.01*(this->vertical_position_corr / this->sum_Ifil_corr);
				
					if(this->division_corr<0){
						this->radial_position_corr = this->clip_limit;
					}else{
						this->radial_position_corr = (0.01*sqrt(this->division_corr))-0.46;
					}
			
				}else{
					this->radial_position_corr = this->clip_limit;
					this->vertical_position_corr =this->clip_limit;
				}
			}
			
			else{
					this->radial_position_corr = this->clip_limit;
					this->vertical_position_corr =this->clip_limit;
					this->radial_position = this->clip_limit;
					this->vertical_position =this->clip_limit;
			}
			
			//
		
				
			

			// Hard clip position (limits for the output signal)
			if(radial_position < -this->clip_limit) radial_position = -this->clip_limit;
			if(radial_position > this->clip_limit) radial_position = this->clip_limit;

			if(vertical_position < -this->clip_limit) vertical_position = -this->clip_limit;
			if(vertical_position > this->clip_limit) vertical_position = this->clip_limit;
			//multifilament
			if(radial_position_corr < -this->clip_limit) radial_position_corr = -this->clip_limit;
			if(radial_position_corr > this->clip_limit) radial_position_corr = this->clip_limit;

			if(vertical_position_corr < -this->clip_limit) vertical_position_corr = -this->clip_limit;
			if(vertical_position_corr > this->clip_limit) vertical_position_corr = this->clip_limit;

			//new
			//B reconstruction | I_filament=Mpf*(Mirnv_B_exp_corr')
			//this-> Bmag_rec=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0}; this-> Bmag_rec_corr=(float[12]){0,0,0,0,0,0,0,0,0,0,0,0};
			
			for(i=0; i<this->numFilament; i++){
				for(j=0; j<12; j++){
					this->m= 12*(i)+j;
					//this->Bmag_rec_buff +=  this->Mfp[m] * (this->Ipf[j]);
					this->Bmag_rec_corr_buff += this->Mfp[m] * (this->Ipf_corr[j]);
					
				}
				
				//this->Bmag_rec[i] = this->Bmag_rec_buff;
				this->Bmag_rec_corr[i] = this->Bmag_rec_corr_buff;
				//this->Bmag_rec_buff = 0.0;
				this->Bmag_rec_corr_buff = 0.0;
				
			}
			
			
			//MIRNOV
			this->Rmse_buff = 0.0;
			this->RMSE_mirnv= 0.0;
			for(i = 0; i < 12; i++) {
				this->Rmse_buff += (Bmag_rec_corr[i] - (this->ADC_final[i]/(50*49*1e-6)) )*(Bmag_rec_corr[i] - (this->ADC_final[i]/(50*49*1e-6)) );
			}
			this->Rmse_buff = this->Rmse_buff / 12;
			this->RMSE_mirnv = sqrt(this->Rmse_buff);
			
			/*//CURRENT
			this->Rmse_buff = 0.0;
			
			this->Rmse_buff = (PlasmaCurrent - sum_Ifil_corr)*(PlasmaCurrent - sum_Ifil_corr) ;
			this->Rmse_buff = sqrt(this->Rmse_buff);
			this->RMSE_Ifil = this->Rmse_buff;*/
			//end

			outputstruct[0].MagneticProbesR = radial_position_corr;
			outputstruct[0].MagneticProbesZ = vertical_position_corr;
			outputstruct[0].Magnetics_R_uncorrctd = radial_position;
			outputstruct[0].Magnetics_z_uncorrctd = vertical_position;
			//new
			// Lets send it to DataBasee in [Wb] to compare with mirnov coils
			outputstruct[0].RMSE_mirnv = this->RMSE_mirnv;
			outputstruct[0].SumIfil = sum_Ifil_corr;
			outputstruct[0].Magnetics_SVD_recons_0 = (50*49*1e-6)*Bmag_rec_corr[0];
			outputstruct[0].Magnetics_SVD_recons_1 = (50*49*1e-6)*Bmag_rec_corr[1];
			outputstruct[0].Magnetics_SVD_recons_2 = (50*49*1e-6)*Bmag_rec_corr[2];
			outputstruct[0].Magnetics_SVD_recons_3 =(50*49*1e-6)* Bmag_rec_corr[3];
			outputstruct[0].Magnetics_SVD_recons_4 = (50*49*1e-6)*Bmag_rec_corr[4];
			outputstruct[0].Magnetics_SVD_recons_5 = (50*49*1e-6)*Bmag_rec_corr[5];
			outputstruct[0].Magnetics_SVD_recons_6 = (50*49*1e-6)*Bmag_rec_corr[6];
			outputstruct[0].Magnetics_SVD_recons_7 = (50*49*1e-6)*Bmag_rec_corr[7];
			outputstruct[0].Magnetics_SVD_recons_8 = (50*49*1e-6)*Bmag_rec_corr[8];
			outputstruct[0].Magnetics_SVD_recons_9 = (50*49*1e-6)*Bmag_rec_corr[9];
			outputstruct[0].Magnetics_SVD_recons_10 = (50*49*1e-6)*Bmag_rec_corr[10];
			outputstruct[0].Magnetics_SVD_recons_11 = (50*49*1e-6)*Bmag_rec_corr[11];
			//end
			

		} // ******** End usectime > usectime_to_wait_for_starting_operation **************
	} // ************* End If(GAMOnline) *******************************************
	else {		// GAMOffline & others
		//this->n_samples = 0;
		//for(i = 0 ; i < (this->NumberOfMeasurements) ; i++){
		//	this->magnetic_Offset_zero[i] = 0;
		//}
		outputstruct[0].MagneticProbesPlasmaCurrent = 0;
		outputstruct[0].MagneticProbesR = 0;
		outputstruct[0].MagneticProbesZ = 0;
		outputstruct[0].Magnetics_R_uncorrctd = 0;
		outputstruct[0].Magnetics_z_uncorrctd = 0;
		
		outputstruct[0].ADC_magnetic_WO_corrctd_0 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_1 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_2 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_3 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_4 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_5 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_6 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_7 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_8 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_9 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_10 = 0.;
		outputstruct[0].ADC_magnetic_WO_corrctd_11 = 0.;
		
		//Write the value of the 12 mirnov external fluxes reconstructed
			outputstruct[0].Magnetics_ext_flux_0 = 0.;
			outputstruct[0].Magnetics_ext_flux_1 = 0.;
			outputstruct[0].Magnetics_ext_flux_2 = 0.;
			outputstruct[0].Magnetics_ext_flux_3 = 0.;
			outputstruct[0].Magnetics_ext_flux_4 = 0.;
			outputstruct[0].Magnetics_ext_flux_5 = 0.;
			outputstruct[0].Magnetics_ext_flux_6 = 0.;
			outputstruct[0].Magnetics_ext_flux_7 = 0.;
			outputstruct[0].Magnetics_ext_flux_8 = 0.;
			outputstruct[0].Magnetics_ext_flux_9 = 0.;
			outputstruct[0].Magnetics_ext_flux_10 = 0.;
			outputstruct[0].Magnetics_ext_flux_11 = 0.;
	
		outputstruct[0].Magnetics_Ip_corrctd = 0;
		
		if (((uint)inputstruct[0].usectime - this->saved_usectime) > 30000000 || ((uint)inputstruct[0].usectime - this->saved_usectime) < 0 ) {
			AssertErrorCondition(Information,"MagneticsGAM:: GAMOffline: usectime: %i ", (uint)inputstruct[0].usectime);
			slope = ((this->ADC_values[0] - this->lastmirnov[0])) / ((float)((uint)inputstruct[0].usectime - this->saved_usectime + 0.1));
			AssertErrorCondition(Information,"MagneticsGAM:: GAMOffline: usectime: %i, ADC_magnetic_chopper_fp_0 : %i, ADC: %f, slope: %f", (uint)inputstruct[0].usectime,inputstruct[0].ADC_magnetic_chopper_fp_0, ADC_values[0],slope);
			lastmirnov[0] = ADC_values[0];
		}
		this->lasttime=inputstruct[0].usectime;
		this->p=0;
		this->saved_usectime = (uint)inputstruct[0].usectime;
		
		for (i = 0; i < this->NumberOfMeasurements; i++) {
				this->lastmirnov[i] = this->ADC_values[i];
			}
		this->mirnovaccumulator = (float[12]) {0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};
		this->accumulatorcounter = 0;
		
	} //	*******	End If(!GAMOnline) *********
	

	this->SignalsOutputInterface->Write();

	return True;
}
// ******************************************************************

bool MagneticsGAM::ProcessHttpMessage(HttpStream &hStream) {

	HtmlStream hmStream(hStream);
	int i,j;
	hmStream.SSPrintf(HtmlTagStreamMode, "html>\n\
		<head>\n\
		<title>%s</title>\n\
		</head>\n\
		<body>\n\
		<svg width=\"100&#37;\" height=\"100\" style=\"background-color: AliceBlue;\">\n\
		<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"%s\" />\n\
		</svg", (char *) this->Name(), 0, 0, 422, 87, "http://www.ipfn.ist.utl.pt/ipfnPortalLayout/themes/ipfn/_img_/logoIPFN_Topo_officialColours.png");

	hmStream.SSPrintf(HtmlTagStreamMode, "br><br><text style=\"font-family:Arial;font-size:46\">%s</text><br", (char *) this->Name());

	FString submit_view;
	submit_view.SetSize(0);
	if (hStream.Switch("InputCommands.submit_view")) {
		hStream.Seek(0);
		hStream.GetToken(submit_view, "");
		hStream.Switch((uint32)0);
	}
	if (submit_view.Size() > 0) view_input_variables = True;

	FString submit_hide;
	submit_hide.SetSize(0);
	if (hStream.Switch("InputCommands.submit_hide")) {
		hStream.Seek(0);
		hStream.GetToken(submit_hide, "");
		hStream.Switch((uint32)0);
	}
	if (submit_hide.Size() > 0) view_input_variables = False;

	hmStream.SSPrintf(HtmlTagStreamMode, "form enctype=\"multipart/form-data\" method=\"post\"");
	if (!view_input_variables) {
		hmStream.SSPrintf(HtmlTagStreamMode, "input type=\"submit\" name=\"submit_view\" value=\"View input variables\"");
	}
	else {
		hmStream.SSPrintf(HtmlTagStreamMode, "input type=\"submit\" name=\"submit_hide\" value=\"Hide input variables\"");
		hmStream.SSPrintf(HtmlTagStreamMode, "br><br>magnetic_radial_bool = %d\n\
		<br>magnetic_vertical_bool = %d\n\
		<br>magnetic_module_correction_bool = %d\n\
		<br>NumberOfProbes = %d\n\
		<br>NumberOfMeasurements = %d\n\
		<br>NumberOfModules = %d\n\
		<br><br", magnetic_radial_bool, magnetic_vertical_bool, magnetic_module_correction_bool, NumberOfProbes, NumberOfMeasurements, NumberOfModules);

		hmStream.SSPrintf(HtmlTagStreamMode, "table border=\"1\"><tr><td>magnetic_Angles</td");
		for (i = 0; i<NumberOfProbes; i++)hmStream.SSPrintf(HtmlTagStreamMode, "td>%.2f</td", magnetic_Angles[i]);
		hmStream.SSPrintf(HtmlTagStreamMode, "/tr><tr><td>magnetic_Calibration</td");
		for (i = 0; i<NumberOfProbes; i++)hmStream.SSPrintf(HtmlTagStreamMode, "td>%.2f</td", magnetic_Calibration[i]);
		hmStream.SSPrintf(HtmlTagStreamMode, "/tr></table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "table border=\"1\"><tr><td>ProbeNumbers</td");
		for (i = 0; i<NumberOfMeasurements; i++)hmStream.SSPrintf(HtmlTagStreamMode, "td>%d</td", ProbeNumbers[i]);
		hmStream.SSPrintf(HtmlTagStreamMode, "/tr></table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "table border=\"1\"><tr><td>magnetic_Offset_slope</td");
		for (i = 0; i<NumberOfModules; i++)hmStream.SSPrintf(HtmlTagStreamMode, "td>%.2f</td", magnetic_Offset_slope[i]);
		hmStream.SSPrintf(HtmlTagStreamMode, "/tr></table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "table border=\"1\"><tr><td>magnetic_Polarity_calibration</td");
		for (i = 0; i<NumberOfModules; i++)hmStream.SSPrintf(HtmlTagStreamMode, "td>%.2f</td", magnetic_Polarity_calibration[i]);
		hmStream.SSPrintf(HtmlTagStreamMode, "/tr></table><br");
		
		
		for (j = 0; j < this->NumberOfMeasurements; j++) {
			hmStream.SSPrintf(HtmlTagStreamMode, "table border=\"1\"><tr><td>Calibration mirnov %d </td", j);
				for (i = 0; i < 29; i++) {
					hmStream.SSPrintf(HtmlTagStreamMode, "td>%.4f</td", slopes[j][i]);
				}
				hmStream.SSPrintf(HtmlTagStreamMode, "/tr><tr><td>Average: %.4f</td", slope_avrg[j]);
				hmStream.SSPrintf(HtmlTagStreamMode, "/tr></table><br");
		}
	}
	hmStream.SSPrintf(HtmlTagStreamMode, "/form");


	hmStream.SSPrintf(HtmlTagStreamMode, "/body>\n</html");
	hStream.SSPrintf("OutputHttpOtions.Content-Type", "text/html;charset=utf-8");
	hStream.WriteReplyHeader(True);


	return True;
}
