#define AUTO_PID_SOFT_LIMIT 10
#define AUTO_PID_MEDIUM_LIMIT 20

#include "ControllerGAM.h"

OBJECTLOADREGISTER(ControllerGAM, "$Id: $")


//  ******** Default constructor ***********************************
ControllerGAM::ControllerGAM(){
	this->SignalsInputInterface = NULL;
	this->SignalsOutputInterface = NULL;
}




// ********* Destructor ********************************************
ControllerGAM::~ControllerGAM()
{
//	if(this->SignalsInputInterface != NULL) delete[] this->SignalsInputInterface ;
//	if(this->SignalsOutputInterface != NULL) delete[] this->SignalsOutputInterface;
}



//{ ********* Initialise the module ********************************
bool ControllerGAM::Initialise(ConfigurationDataBase& cdbData){

	CDBExtended cdb(cdbData);
	int i;


	if(!cdb.ReadInt32(usecthread_cycle_time, "usecthread_cycle_time"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s usecthread_cycle_time",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: usecthread_cycle_time= %d",usecthread_cycle_time);
	if(!cdb.ReadFloat(maximum_horizontal_current, "maximum_horizontal_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_horizontal_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_horizontal_current = %f",maximum_horizontal_current);
	if(!cdb.ReadFloat(minimum_horizontal_current, "minimum_horizontal_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_horizontal_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_horizontal_current = %f",minimum_horizontal_current);
	if(!cdb.ReadFloat(maximum_vertical_current, "maximum_vertical_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_vertical_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_vertical_current = %f",maximum_vertical_current);
	if(!cdb.ReadFloat(minimum_vertical_current, "minimum_vertical_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_vertical_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_vertical_current = %f",minimum_vertical_current);
	if(!cdb.ReadFloat(maximum_primary_current, "maximum_primary_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_primary_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_primary_current = %f",maximum_primary_current);
	if(!cdb.ReadFloat(minimum_primary_current, "minimum_primary_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_primary_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_primary_current = %f",minimum_primary_current);

	if(!cdb.ReadFloat(maximum_horizontal_position, "maximum_horizontal_position"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_horizontal_position",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_horizontal_position = %f",maximum_horizontal_position);
	if(!cdb.ReadFloat(minimum_horizontal_position, "minimum_horizontal_position"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_horizontal_position",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_horizontal_position = %f",minimum_horizontal_position);
	if(!cdb.ReadFloat(maximum_vertical_position, "maximum_vertical_position"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_vertical_position",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_vertical_position = %f",maximum_vertical_position);
	if(!cdb.ReadFloat(minimum_vertical_position, "minimum_vertical_position"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_vertical_position",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_vertical_position = %f",minimum_vertical_position);
	if(!cdb.ReadFloat(maximum_plasma_current, "maximum_plasma_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_plasma_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_plasma_current = %f",maximum_plasma_current);
	if(!cdb.ReadFloat(minimum_plasma_current, "minimum_plasma_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_plasma_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_plasma_current = %f",minimum_plasma_current);
	if(!cdb.ReadFloat(maximum_toroidal_current, "maximum_toroidal_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_toroidal_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_toroidal_current = %f",maximum_toroidal_current);
	if(!cdb.ReadFloat(minimum_toroidal_current, "minimum_toroidal_current"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_toroidal_current",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_toroidal_current = %f",minimum_toroidal_current);
	if(!cdb.ReadFloat(maximum_puffing_output, "maximum_puffing_output"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_puffing_output",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_puffing_output = %f",maximum_puffing_output);
	if(!cdb.ReadFloat(minimum_puffing_output, "minimum_puffing_output"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_puffing_output",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_puffing_output = %f",minimum_puffing_output);
	if(!cdb.ReadFloat(maximum_density_halpha_scenario, "maximum_density_halpha_scenario"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_density_halpha_scenario",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_density_halpha_scenario = %f",maximum_density_halpha_scenario);
	if(!cdb.ReadFloat(minimum_density_halpha_scenario, "minimum_density_halpha_scenario"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_density_halpha_scenario",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_density_halpha_scenario = %f",minimum_density_halpha_scenario);
	if(!cdb.ReadInt32(i, "interferometry_radial_control_bool"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s interferometry_radial_control_bool",this->Name());
		return False;
	}
	else
	{
		interferometry_radial_control_bool = (bool)i;
		AssertErrorCondition(Information,"ControllerGAM::Initialise: interferometry_radial_control_bool = %d",interferometry_radial_control_bool);
	}
	if(!cdb.ReadFloat(puffing_duration_in_puffing_feedback_in_ms, "puffing_duration_in_puffing_feedback_in_ms"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s puffing_duration_in_puffing_feedback_in_ms",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: puffing_duration_in_puffing_feedback_in_ms = %f",puffing_duration_in_puffing_feedback_in_ms);
	if(!cdb.ReadFloat(maximum_idle_time_in_puffing_feedback_in_ms, "maximum_idle_time_in_puffing_feedback_in_ms"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s maximum_idle_time_in_puffing_feedback_in_ms",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: maximum_idle_time_in_puffing_feedback_in_ms = %f",maximum_idle_time_in_puffing_feedback_in_ms);
	if(!cdb.ReadFloat(minimum_idle_time_in_puffing_feedback_in_ms, "minimum_idle_time_in_puffing_feedback_in_ms"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s minimum_idle_time_in_puffing_feedback_in_ms",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: minimum_idle_time_in_puffing_feedback_in_ms = %f",minimum_idle_time_in_puffing_feedback_in_ms);
	if(!cdb.ReadFloat(puffing_feedback_usec_change_percentage_by_cycle, "puffing_feedback_usec_change_percentage_by_cycle"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s puffing_feedback_usec_change_percentage_by_cycle",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: puffing_feedback_usec_change_percentage_by_cycle = %f",puffing_feedback_usec_change_percentage_by_cycle);
	if(!cdb.ReadInt32(puffing_feedback_mode, "puffing_feedback_mode"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s puffing_feedback_mode",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: puffing_feedback_mode = %d",puffing_feedback_mode);
	if(!cdb.ReadInt32(puffing_mode, "puffing_mode"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s puffing_mode",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: puffing_mode = %d",puffing_mode);

	if(!cdb->Move("PID_horizontal"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"PID_horizontal\"",this->Name());
		return False;
	}
		if(!cdb.ReadFloat(PID_horizontal_proportional_soft, "proportional_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_proportional_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_proportional_soft = %f",PID_horizontal_proportional_soft);
		if(!cdb.ReadFloat(PID_horizontal_proportional_normal, "proportional_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_proportional_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_proportional_normal = %f",PID_horizontal_proportional_normal);
		if(!cdb.ReadFloat(PID_horizontal_proportional_hard, "proportional_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_proportional_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_proportional_hard = %f",PID_horizontal_proportional_hard);
		if(!cdb.ReadFloat(PID_horizontal_integral_soft, "integral_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_integral_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_integral_soft = %f",PID_horizontal_integral_soft);
		if(!cdb.ReadFloat(PID_horizontal_integral_normal, "integral_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_integral_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_integral_normal = %f",PID_horizontal_integral_normal);
		if(!cdb.ReadFloat(PID_horizontal_integral_hard, "integral_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_integral_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_integral_hard = %f",PID_horizontal_integral_hard);
		if(!cdb.ReadFloat(PID_horizontal_derivative_soft, "derivative_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_derivative_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_derivative_soft = %f",PID_horizontal_derivative_soft);
		if(!cdb.ReadFloat(PID_horizontal_derivative_normal, "derivative_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_derivative_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_derivative_normal = %f",PID_horizontal_derivative_normal);
		if(!cdb.ReadFloat(PID_horizontal_derivative_hard, "derivative_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_horizontal_derivative_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_horizontal_derivative_hard = %f",PID_horizontal_derivative_hard);

	cdb->MoveToFather();

	if(!cdb->Move("PID_vertical"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"PID_vertical\"",this->Name());
		return False;
	}
		if(!cdb.ReadFloat(PID_vertical_proportional_soft, "proportional_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_proportional_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_proportional_soft = %f",PID_vertical_proportional_soft);
		if(!cdb.ReadFloat(PID_vertical_proportional_normal, "proportional_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_proportional_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_proportional_normal = %f",PID_vertical_proportional_normal);
		if(!cdb.ReadFloat(PID_vertical_proportional_hard, "proportional_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_proportional_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_proportional_hard = %f",PID_vertical_proportional_hard);
		if(!cdb.ReadFloat(PID_vertical_integral_soft, "integral_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_integral_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_integral_soft = %f",PID_vertical_integral_soft);
		if(!cdb.ReadFloat(PID_vertical_integral_normal, "integral_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_integral_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_integral_normal = %f",PID_vertical_integral_normal);
		if(!cdb.ReadFloat(PID_vertical_integral_hard, "integral_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_integral_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_integral_hard = %f",PID_vertical_integral_hard);
		if(!cdb.ReadFloat(PID_vertical_derivative_soft, "derivative_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_derivative_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_derivative_soft = %f",PID_vertical_derivative_soft);
		if(!cdb.ReadFloat(PID_vertical_derivative_normal, "derivative_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_derivative_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_derivative_normal = %f",PID_vertical_derivative_normal);
		if(!cdb.ReadFloat(PID_vertical_derivative_hard, "derivative_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_vertical_derivative_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_vertical_derivative_hard = %f",PID_vertical_derivative_hard);

	cdb->MoveToFather();

	if(!cdb->Move("PID_primary"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"PID_primary\"",this->Name());
		return False;
	}
		if(!cdb.ReadFloat(PID_primary_proportional_soft, "proportional_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_proportional_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_proportional_soft = %f",PID_primary_proportional_soft);
		if(!cdb.ReadFloat(PID_primary_proportional_normal, "proportional_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_proportional_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_proportional_normal = %f",PID_primary_proportional_normal);
		if(!cdb.ReadFloat(PID_primary_proportional_hard, "proportional_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_proportional_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_proportional_hard = %f",PID_primary_proportional_hard);
		if(!cdb.ReadFloat(PID_primary_integral_soft, "integral_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_integral_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_integral_soft = %f",PID_primary_integral_soft);
		if(!cdb.ReadFloat(PID_primary_integral_normal, "integral_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_integral_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_integral_normal = %f",PID_primary_integral_normal);
		if(!cdb.ReadFloat(PID_primary_integral_hard, "integral_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_integral_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_integral_hard = %f",PID_primary_integral_hard);
		if(!cdb.ReadFloat(PID_primary_derivative_soft, "derivative_soft"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_derivative_soft",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_derivative_soft = %f",PID_primary_derivative_soft);
		if(!cdb.ReadFloat(PID_primary_derivative_normal, "derivative_normal"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_derivative_normal",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_derivative_normal = %f",PID_primary_derivative_normal);
		if(!cdb.ReadFloat(PID_primary_derivative_hard, "derivative_hard"))
		{
			AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s PID_primary_derivative_hard",this->Name());
			return False;
		}
		else AssertErrorCondition(Information,"ControllerGAM::Initialise: PID_primary_derivative_hard = %f",PID_primary_derivative_hard);

	cdb->MoveToFather();

//	sleep(3);
// Create the signal interfaces
	if(!AddInputInterface(this->SignalsInputInterface, "ControllerGAMInputInterface"))
	{
		AssertErrorCondition(InitialisationError, "ControllerGAM::Initialise: %s failed to add the TimewindowsGAMInputInterface", this->Name());
		return False;
	}
	if(!AddOutputInterface(this->SignalsOutputInterface, "ControllerGAMOutputInterface"))
	{
		AssertErrorCondition(InitialisationError, "ControllerGAM::Initialise: %s failed to add the TimewindowsGAMOutputInterface", this->Name());
		return False;
	}

//	INPUT SIGNALS (interface)
	if(!cdb->Move("input_signals"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"input_signals\"",this->Name());
		return False;
	}
		int number_of_signals_to_read = 19;
		FString *CDB_move_to;
		FString *SignalType;
		CDB_move_to = new FString[number_of_signals_to_read];
		SignalType = new FString[number_of_signals_to_read];
		CDB_move_to[0].Printf("primary_current");
		CDB_move_to[1].Printf("horizontal_current");
		CDB_move_to[2].Printf("vertical_current");
		CDB_move_to[3].Printf("primary_waveform");
		CDB_move_to[4].Printf("horizontal_waveform");
		CDB_move_to[5].Printf("vertical_waveform");
		CDB_move_to[6].Printf("puffing_waveform");
		CDB_move_to[7].Printf("toroidal_waveform");
		CDB_move_to[8].Printf("plasma_current");
		CDB_move_to[9].Printf("position_r");
		CDB_move_to[10].Printf("position_z");
		CDB_move_to[11].Printf("density");
		CDB_move_to[12].Printf("halpha");
		CDB_move_to[13].Printf("interferometry_radial_position");
		CDB_move_to[14].Printf("primary_mode");
		CDB_move_to[15].Printf("horizontal_mode");
		CDB_move_to[16].Printf("vertical_mode");
		CDB_move_to[17].Printf("time");
		CDB_move_to[18].Printf("discharge_status");
		for (i=0;i<number_of_signals_to_read;i++){

			if(!cdb->Move(CDB_move_to[i].Buffer()))
			{
				AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"%s\"",this->Name(),CDB_move_to[i].Buffer());
				return False;
			}

			if(cdb->Exists("SignalType"))
			{
				FString signalName;
				cdb.ReadFString(SignalType[i], "SignalType");
			}
			if(cdb->Exists("SignalName"))
			{
				FString SignalName;
				cdb.ReadFString(SignalName, "SignalName");
				AssertErrorCondition(Information,"ControllerGAM::Initialise: Added signal = %s", SignalName.Buffer());

				if(!this->SignalsInputInterface->AddSignal(SignalName.Buffer(), SignalType[i].Buffer()))
				{
					AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s failed to add signal", this->Name());
					return False;
				}
			}


			cdb->MoveToFather();
		}

	cdb->MoveToFather();


//	OUTPUT SIGNALS (interface)
	if(!cdb->Move("output_signals"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"output_signals\"",this->Name());
		return False;
	}

		number_of_signals_to_read = 7;
		CDB_move_to = new FString[number_of_signals_to_read];
		SignalType = new FString[number_of_signals_to_read];
		CDB_move_to[0].Printf("output_horizontal");
		CDB_move_to[1].Printf("output_vertical");
		CDB_move_to[2].Printf("output_primary");
		CDB_move_to[3].Printf("output_Puffing");
		CDB_move_to[4].Printf("output_Toroidal");
		CDB_move_to[5].Printf("R_recons");
		CDB_move_to[6].Printf("Z_recons");
		for (i=0;i<number_of_signals_to_read;i++){

			if(!cdb->Move(CDB_move_to[i].Buffer()))
			{
				AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not move to \"%s\"",this->Name(),CDB_move_to[i].Buffer());
				return False;
			}

			if(cdb->Exists("SignalType"))
			{
				FString signalName;
				cdb.ReadFString(SignalType[i], "SignalType");
			}
			if(cdb->Exists("SignalName"))
			{
				FString SignalName;
				cdb.ReadFString(SignalName, "SignalName");
				AssertErrorCondition(Information,"ControllerGAM::Initialise: Added signal = %s", SignalName.Buffer());

				if(!this->SignalsOutputInterface->AddSignal(SignalName.Buffer(), SignalType[i].Buffer()))
				{
					AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s failed to add signal", this->Name());
					return False;
				}
			}
			cdb->MoveToFather();
		}

	cdb->MoveToFather();


//	READ control file
	File temp_file;
	FString file_to_read;
	ConfigurationDataBase file_cdb;

	if(!cdb.ReadFString(file_to_read, "file_to_load"))
	{
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: %s Could not get file_to_load",this->Name());
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: file_to_load = %s",file_to_read.Buffer());

   if(!temp_file.OpenRead(file_to_read.Buffer())){
        AssertErrorCondition(InitialisationError, "ControllerGAM::Initialise: Failed opening File %s", file_to_read.Buffer() );
//		temp_file.Close();
        return False;
    }

	file_cdb->ReadFromStream(temp_file);
	CDBExtended cdbe(file_cdb);

	temp_max_dim = 2;

	if (!cdbe->GetArrayDims(A_matrix_dims,temp_max_dim,"A_matrix",CDBAIM_Strict)){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: Could not get A_matrix dimension");
		temp_file.Close();
		return False;
	}
	if (temp_max_dim != 2 || A_matrix_dims[0] == 0 || A_matrix_dims[1] == 0){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: A_matrix dimension != 2");
		temp_file.Close();
		return False;
	}
	A_matrix.ReSize(A_matrix_dims[0],A_matrix_dims[1]);
	if (!cdbe.ReadFloatArray((float *)A_matrix.data,A_matrix_dims,2,"A_matrix")){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: could not read A_matrix matrix");
		temp_file.Close();
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: successfully loaded A_matrix matrix size = %d , %d", A_matrix_dims[0],A_matrix_dims[1]);

	if (!cdbe->GetArrayDims(B_matrix_dims,temp_max_dim,"B_matrix",CDBAIM_Strict)){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: Could not get B_matrix dimension");
		temp_file.Close();
		return False;
	}
	if (temp_max_dim != 2 || B_matrix_dims[0] == 0 || B_matrix_dims[1] == 0){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: B_matrix dimension != 2");
		temp_file.Close();
		return False;
	}
	B_matrix.ReSize(B_matrix_dims[0],B_matrix_dims[1]);
	if (!cdbe.ReadFloatArray((float *)B_matrix.data,B_matrix_dims,2,"B_matrix")){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: could not read B_matrix matrix");
		temp_file.Close();
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: successfully loaded B_matrix matrix size = %d , %d", B_matrix_dims[0],B_matrix_dims[1]);

	if (!cdbe->GetArrayDims(C_matrix_dims,temp_max_dim,"C_matrix",CDBAIM_Strict)){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: Could not get C_matrix dimension");
		temp_file.Close();
		return False;
	}
	if (temp_max_dim != 2 || C_matrix_dims[0] == 0 || C_matrix_dims[1] == 0){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: C_matrix dimension != 2");
		temp_file.Close();
		return False;
	}
	C_matrix.ReSize(C_matrix_dims[0],C_matrix_dims[1]);
	if (!cdbe.ReadFloatArray((float *)C_matrix.data,C_matrix_dims,2,"C_matrix")){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: could not read C_matrix matrix");
		temp_file.Close();
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: successfully loaded C_matrix matrix size = %d , %d", C_matrix_dims[0],C_matrix_dims[1]);

	if (!cdbe->GetArrayDims(D_matrix_dims,temp_max_dim,"D_matrix",CDBAIM_Strict)){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: Could not get D_matrix dimension");
		temp_file.Close();
		return False;
	}
	if (temp_max_dim != 2 || D_matrix_dims[0] == 0 || D_matrix_dims[1] == 0){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: D_matrix dimension != 2");
		temp_file.Close();
		return False;
	}
	D_matrix.ReSize(D_matrix_dims[0],D_matrix_dims[1]);
	if (!cdbe.ReadFloatArray((float *)D_matrix.data,D_matrix_dims,2,"D_matrix")){
		AssertErrorCondition(InitialisationError,"ControllerGAM::Initialise: could not read D_matrix matrix");
		temp_file.Close();
		return False;
	}
	else AssertErrorCondition(Information,"ControllerGAM::Initialise: successfully loaded D_matrix matrix size = %d , %d", D_matrix_dims[0],D_matrix_dims[1]);

	temp_file.Close();

	this->PID_time_constant = usecthread_cycle_time / 1000000;
	this->PID_time_constant=0.0001;

	this->horizontal_position_PID = new IPID(this->PID_horizontal_proportional_normal, this->PID_horizontal_integral_normal, this->PID_horizontal_derivative_normal,this->PID_time_constant, this->maximum_vertical_current, this->minimum_vertical_current );
	this->vertical_position_PID = new IPID(this->PID_vertical_proportional_normal, this->PID_vertical_integral_normal, this->PID_vertical_derivative_normal,this->PID_time_constant, this->maximum_horizontal_current, this->minimum_horizontal_current);
	this->primary_plasma_current_PID = new IPID(this->PID_primary_proportional_normal, this->PID_primary_integral_normal, this->PID_primary_derivative_normal,this->PID_time_constant, this->maximum_primary_current, this->minimum_primary_current );

	// Chamar construtor do  LQR

	this->Kalman_LQR_var = new LQR();

	this->CentroidPos.Kalman_R=0;
	this->CentroidPos.Kalman_Z=0;
	this->CentroidPos.X_est=(float[10]){0, 0,0,0,0,0,0,0,0,0};
	this->LQRcurrents.Ivert=0.0;
	this->LQRcurrents.Ihor=0.0;

	this-> Radial_pos=0;
	this-> Vertical_pos=0;
	this-> IVertical=0;
	this-> IHorizontal=0;
	this-> changeDetec=0;
	this-> firstcycle=0;
	this-> SendToVer_buff=0;
	this-> SendToHor_buff=0;

	puffing_duration_in_puffing_feedback_in_us = int(puffing_duration_in_puffing_feedback_in_ms * 1000);
	maximum_idle_time_in_puffing_feedback_in_us = int(maximum_idle_time_in_puffing_feedback_in_ms * 1000);
	minimum_idle_time_in_puffing_feedback_in_us =int(minimum_idle_time_in_puffing_feedback_in_ms * 1000);

	puffing_feedback_last_usectime = 0;
	puffing_feedback_usectime_to_change = minimum_idle_time_in_puffing_feedback_in_us;
	puffing_feedback_currently_off = True;				//0 -> puffing feedback on, waiting to turn off, 1 -> puffing feedback off, waiting to turn on
	puffing_feedback_usec_change_per_cycle = int((0.5 + usecthread_cycle_time * puffing_feedback_usec_change_percentage_by_cycle / 100 ) );
	if (puffing_feedback_usec_change_per_cycle < 1) puffing_feedback_usec_change_per_cycle = 1;

	old_PrimaryWaveformMode = 0;
	old_VerticalWaveformMode = 0;
	old_HorizontalWaveformMode = 0;
	old_DischargeStatus = -1;



	//AssertErrorCondition(Information,"ControllerGAM::Initialise: puffing change per cycle = %d ", puffing_feedback_usec_change_per_cycle);

    return True;
}
//} ******************************************************************



//{ ********* Execute the module functionalities *******************
bool ControllerGAM::Execute(GAM_FunctionNumbers functionNumber){


	InputInterfaceStruct *inputstruct = (InputInterfaceStruct *) this->SignalsInputInterface->Buffer();
	this->SignalsInputInterface->Read();
//	AssertErrorCondition(InitialisationError,"ControllerGAM:: %s inputstruct = %f %f %f %f %f %f %f %f %f %f %f %d %d %d %d ",this->Name(), inputstruct[0].PrimaryCurrent, inputstruct[0].HorizontalCurrent, inputstruct[0].VerticalCurrent, inputstruct[0].PrimaryOutputWaveform, inputstruct[0].HorizontalOutputWaveform, inputstruct[0].VerticalOutputWaveform, inputstruct[0].PlasmaCurrent, inputstruct[0].PositionR, inputstruct[0].PositionZ, inputstruct[0].Density, inputstruct[0].InterferometryR, inputstruct[0].PrimaryWaveformMode, inputstruct[0].HorizontalWaveformMode, inputstruct[0].VerticalWaveformMode, inputstruct[0].usecTime);

	OutputInterfaceStruct *outputstruct = (OutputInterfaceStruct *) this->SignalsOutputInterface->Buffer();


	Radial_pos= inputstruct[0].PositionR;
	Vertical_pos= inputstruct[0].PositionZ;
	IVertical= inputstruct[0].VerticalCurrent;
	IHorizontal= inputstruct[0].HorizontalCurrent;


/* **** WaveformModes of operation *****
	0 -> off
	1 -> current control
	2 -> position soft
	3 -> position medium
	4 -> position hard
	5 -> position auto
	6 -> integrated*
	7 -> invert*
*/
/*
	*** puffing_mode ***
	0 -> off
	1 -> time-windows, out percentage
	2 -> time windows density feedback
	3 -> absolute time, out percentage
	4 -> before breakdown - > open loop (use absolute time waveform), after breakdown feedback in time-windows.
*/
/*
	*** puffing_feedback_mode ***
	1 -> feedback in density
	2 -> feedback in HAlpha
*/
/*
NOT USED FOR NOW      vertical field PS - radial position controller (auto)
                      Ivert = PIDout x IP + 0.04 x IP
*/

	if(functionNumber == GAMOffline){
		outputstruct[0].SendToHorizontalValue = 0;
		outputstruct[0].SendToVerticalValue = 0;
		outputstruct[0].SendToPrimaryValue = 0;
		outputstruct[0].SendToPuffing = 0;
		outputstruct[0].SendToToroidal = 0;
		outputstruct[0].R_recons=0;
		outputstruct[0].Z_recons=0;
	}

	if(functionNumber == GAMOnline){




		if (inputstruct[0].DischargeStatus >=0 ){


//////// Get initial conditions
/*
if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750 ) {
	if(this->firstcycle== 0){
			int ivo;
			ivo = this->Kalman_LQR_var->erase(Radial_pos, Vertical_pos);
	        this->firstcycle =1;}
	}
else if ( inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750){
	if(this->firstcycle== 0){
			int ivo;
			ivo = this->Kalman_LQR_var->erase(Radial_pos, Vertical_pos);
	        this->firstcycle =1;}
	}	else{this->firstcycle =0;}
*/
	//////////////////////////////////////


			if (inputstruct[0].PrimaryWaveformMode == 7 || inputstruct[0].HorizontalWaveformMode == 7 || inputstruct[0].VerticalWaveformMode == 7){

				float temp;

				outputstruct[0].SendToPrimaryValue = inputstruct[0].PrimaryCurrent;
				this->SendToVer_buff = inputstruct[0].VerticalCurrent;
				this->SendToHor_buff = inputstruct[0].HorizontalCurrent;

				old_PrimaryWaveformMode = 7;
				old_VerticalWaveformMode = 7;
				old_HorizontalWaveformMode = 7;

				// bumpless condition to -1
				 temp =	 this->horizontal_position_PID->CalculatePID_vert(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000),2,-1);
				temp =	 this->vertical_position_PID->CalculatePID_hor(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000),2,-1);


			}

			else if (inputstruct[0].PrimaryWaveformMode == 6 || inputstruct[0].HorizontalWaveformMode == 6 || inputstruct[0].VerticalWaveformMode == 6){

				//integrated control
				if (inputstruct[0].PlasmaCurrent < inputstruct[0].PrimaryOutputWaveform) outputstruct[0].SendToPrimaryValue += (this->maximum_primary_current - this->minimum_primary_current)/400;
				else outputstruct[0].SendToPrimaryValue -= (this->maximum_primary_current - this->minimum_primary_current)/400;

				if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750){

					if (inputstruct[0].PositionR > (inputstruct[0].VerticalOutputWaveform/1000)) this->SendToVer_buff += (this->maximum_vertical_current - this->minimum_vertical_current )/200;
					else this->SendToVer_buff -= (this->maximum_vertical_current - this->minimum_vertical_current )/200;
					if (inputstruct[0].PositionZ > (inputstruct[0].HorizontalOutputWaveform/1000)) this->SendToHor_buff -= (this->maximum_horizontal_current-this->minimum_horizontal_current)/1000;
					else this->SendToHor_buff += (this->maximum_horizontal_current-this->minimum_horizontal_current)/1000;
				}
				if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent > -750){
					if (inputstruct[0].PositionR < (inputstruct[0].VerticalOutputWaveform/1000)) this->SendToVer_buff += (this->maximum_vertical_current - this->minimum_vertical_current )/200;
					else this->SendToVer_buff -= (this->maximum_vertical_current - this->minimum_vertical_current )/200;
					if (inputstruct[0].PositionZ < (inputstruct[0].HorizontalOutputWaveform/1000)) this->SendToHor_buff -= (this->maximum_horizontal_current-this->minimum_horizontal_current)/1000;
					else this->SendToHor_buff += (this->maximum_horizontal_current-this->minimum_horizontal_current)/1000;
				}

				if ( outputstruct[0].SendToPrimaryValue > this->maximum_primary_current ) outputstruct[0].SendToPrimaryValue = this->maximum_primary_current;
				if ( outputstruct[0].SendToPrimaryValue < this->minimum_primary_current ) outputstruct[0].SendToPrimaryValue = this->minimum_primary_current;
				if ( this->SendToVer_buff > this->maximum_vertical_current ) this->SendToVer_buff = this->maximum_vertical_current;
				if ( this->SendToVer_buff < this->minimum_horizontal_current ) this->SendToVer_buff = this->minimum_horizontal_current;
				if ( this->SendToHor_buff > this->maximum_horizontal_current ) this->SendToHor_buff = this->maximum_horizontal_current;
				if ( this->SendToHor_buff < this->minimum_horizontal_current ) this->SendToHor_buff = this->minimum_horizontal_current;
				old_PrimaryWaveformMode = 6;
				old_VerticalWaveformMode = 6;
				old_HorizontalWaveformMode = 6;
			}
			else { //// the big else


				if (inputstruct[0].PrimaryWaveformMode == 5){

					// decide wich PID to use based on the current error
					temp_requested_output = this->primary_plasma_current_PID->ReturnErrorInPercentage(inputstruct[0].PlasmaCurrent, inputstruct[0].PrimaryOutputWaveform);
					if (temp_requested_output < AUTO_PID_SOFT_LIMIT) this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_soft, this->PID_primary_integral_soft, this->PID_primary_derivative_soft, this->usecthread_cycle_time);
					else if (temp_requested_output < AUTO_PID_MEDIUM_LIMIT) this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_normal, this->PID_primary_integral_normal, this->PID_primary_derivative_normal, this->usecthread_cycle_time);
					else this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_hard, this->PID_primary_integral_hard, this->PID_primary_derivative_hard, this->usecthread_cycle_time);

					if(old_PrimaryWaveformMode > 5 || old_PrimaryWaveformMode < 2 ) this->primary_plasma_current_PID->LoadOldOutputWithinLimits(inputstruct[0].PrimaryCurrent);

					outputstruct[0].SendToPrimaryValue = this->primary_plasma_current_PID->CalculatePID(inputstruct[0].PlasmaCurrent,inputstruct[0].PrimaryOutputWaveform);

					old_PrimaryWaveformMode = 5;
				}
				if (inputstruct[0].VerticalWaveformMode == 5){

					// decide wich PID to use based on the current error
					temp_requested_output = this->horizontal_position_PID->ReturnErrorInPercentage(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000));
					if (temp_requested_output < AUTO_PID_SOFT_LIMIT) this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_soft, this->PID_vertical_integral_soft, this->PID_vertical_derivative_soft, this->usecthread_cycle_time);
					else if (temp_requested_output < AUTO_PID_MEDIUM_LIMIT) this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_normal, this->PID_vertical_integral_normal, this->PID_vertical_derivative_normal, this->usecthread_cycle_time);
					else this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_hard, this->PID_vertical_integral_hard, this->PID_vertical_derivative_hard, this->usecthread_cycle_time);

					if(old_VerticalWaveformMode > 5 || old_VerticalWaveformMode < 2 ) this->horizontal_position_PID->LoadOldOutputWithinLimits((inputstruct[0].VerticalCurrent));

					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID((2 *(inputstruct[0].VerticalOutputWaveform/1000) - inputstruct[0].PositionR),(inputstruct[0].VerticalOutputWaveform/1000));
					}
					if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000));
					}
					// else: keep the output (no changes)

					old_VerticalWaveformMode = 5;
				}
				if (inputstruct[0].HorizontalWaveformMode == 5){

					// decide wich PID to use based on the current error
					temp_requested_output = this->vertical_position_PID->ReturnErrorInPercentage(inputstruct[0].PositionZ,(inputstruct[0].HorizontalOutputWaveform/1000));
					if (temp_requested_output < AUTO_PID_SOFT_LIMIT) this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_soft, this->PID_horizontal_integral_soft, this->PID_horizontal_derivative_soft, this->usecthread_cycle_time);
					else if (temp_requested_output < AUTO_PID_MEDIUM_LIMIT) this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_normal, this->PID_horizontal_integral_normal, this->PID_horizontal_derivative_normal, this->usecthread_cycle_time);
					else this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_hard, this->PID_horizontal_integral_hard, this->PID_horizontal_derivative_hard, this->usecthread_cycle_time);

					if(old_HorizontalWaveformMode > 5 || old_HorizontalWaveformMode < 2 ) this->vertical_position_PID->LoadOldOutputWithinLimits(inputstruct[0].HorizontalCurrent);

						if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
							this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types((2 * inputstruct[0].HorizontalOutputWaveform/1000 - inputstruct[0].PositionZ), inputstruct[0].HorizontalOutputWaveform/1000, 1);
						}
						if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
							this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types(inputstruct[0].PositionZ, inputstruct[0].HorizontalOutputWaveform/1000, 1);
						}
						// else: keep the output (no changes)

					old_HorizontalWaveformMode = 5;
				}
				if (inputstruct[0].PrimaryWaveformMode == 4){

					if(old_PrimaryWaveformMode != 4) this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_hard, this->PID_primary_integral_hard, this->PID_primary_derivative_hard, this->usecthread_cycle_time);

					if(old_PrimaryWaveformMode > 5 || old_PrimaryWaveformMode < 2 ) this->primary_plasma_current_PID->LoadOldOutputWithinLimits(inputstruct[0].PrimaryCurrent);

					outputstruct[0].SendToPrimaryValue = this->primary_plasma_current_PID->CalculatePID(inputstruct[0].PlasmaCurrent,inputstruct[0].PrimaryOutputWaveform);

					old_PrimaryWaveformMode = 4;
				}
				if (inputstruct[0].VerticalWaveformMode == 4){

					if(old_VerticalWaveformMode != 4) this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_hard, this->PID_vertical_integral_hard, this->PID_vertical_derivative_hard, this->usecthread_cycle_time);

					if(old_VerticalWaveformMode > 5 || old_VerticalWaveformMode < 2 ) this->horizontal_position_PID->LoadOldOutputWithinLimits((inputstruct[0].VerticalCurrent));

					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID((2 *(inputstruct[0].VerticalOutputWaveform/1000) - inputstruct[0].PositionR),(inputstruct[0].VerticalOutputWaveform/1000));
					}
					if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000));
					}
					// else: keep the output (no changes)

					old_VerticalWaveformMode = 4;
				}
				if (inputstruct[0].HorizontalWaveformMode == 4){

					if(old_HorizontalWaveformMode != 4) this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_hard, this->PID_horizontal_integral_hard, this->PID_horizontal_derivative_hard, this->usecthread_cycle_time);

					if(old_HorizontalWaveformMode > 5 || old_HorizontalWaveformMode < 2 ) this->vertical_position_PID->LoadOldOutputWithinLimits(inputstruct[0].HorizontalCurrent);

					if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
						this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types((2 * inputstruct[0].HorizontalOutputWaveform/1000 - inputstruct[0].PositionZ), inputstruct[0].HorizontalOutputWaveform/1000, 1);
					}
					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
						this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types(inputstruct[0].PositionZ, inputstruct[0].HorizontalOutputWaveform/1000, 1);
					}
					// else: keep the output (no changes)

					old_HorizontalWaveformMode = 4;
				}
				if (inputstruct[0].PrimaryWaveformMode == 3){

					if(old_PrimaryWaveformMode != 3) this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_normal, this->PID_primary_integral_normal, this->PID_primary_derivative_normal, this->usecthread_cycle_time);

					if(old_PrimaryWaveformMode > 5 || old_PrimaryWaveformMode < 2 ) this->primary_plasma_current_PID->LoadOldOutputWithinLimits(inputstruct[0].PrimaryCurrent);

					outputstruct[0].SendToPrimaryValue = this->primary_plasma_current_PID->CalculatePID(inputstruct[0].PlasmaCurrent,inputstruct[0].PrimaryOutputWaveform);

					old_PrimaryWaveformMode = 3;
				}
				if (inputstruct[0].VerticalWaveformMode == 3){

					if(old_VerticalWaveformMode != 3) this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_normal, this->PID_vertical_integral_normal, this->PID_vertical_derivative_normal, this->usecthread_cycle_time);

					if(old_VerticalWaveformMode > 5 || old_VerticalWaveformMode < 2 ) this->horizontal_position_PID->LoadOldOutputWithinLimits((inputstruct[0].VerticalCurrent));

					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID((2 *(inputstruct[0].VerticalOutputWaveform/1000) - inputstruct[0].PositionR),(inputstruct[0].VerticalOutputWaveform/1000));
					}
					if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
						this->SendToVer_buff = this->horizontal_position_PID->CalculatePID(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000));
					}
					// else: keep the output (no changes)

					old_VerticalWaveformMode = 3;
				}
				if (inputstruct[0].HorizontalWaveformMode == 3){

					if(old_HorizontalWaveformMode != 3) this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_normal, this->PID_horizontal_integral_normal, this->PID_horizontal_derivative_normal, this->usecthread_cycle_time);

					if(old_HorizontalWaveformMode > 5 || old_HorizontalWaveformMode < 2 ) this->vertical_position_PID->LoadOldOutputWithinLimits(inputstruct[0].HorizontalCurrent);

					if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750) {
						this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types((2 * inputstruct[0].HorizontalOutputWaveform/1000 - inputstruct[0].PositionZ), inputstruct[0].HorizontalOutputWaveform/1000, 1);
					}
					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) {
						this->SendToHor_buff = this->vertical_position_PID->CalculatePID_types(inputstruct[0].PositionZ, inputstruct[0].HorizontalOutputWaveform/1000, 1);
					}
					// else: keep the output (no changes)

					old_HorizontalWaveformMode = 3;
				}
				if (inputstruct[0].PrimaryWaveformMode == 2){

					if(old_PrimaryWaveformMode != 2) this->primary_plasma_current_PID->SetPIDConstants(this->PID_primary_proportional_soft, this->PID_primary_integral_soft, this->PID_primary_derivative_soft, this->usecthread_cycle_time);

					if(old_PrimaryWaveformMode > 5 || old_PrimaryWaveformMode < 2 ) this->primary_plasma_current_PID->LoadOldOutputWithinLimits(inputstruct[0].PrimaryCurrent);

					outputstruct[0].SendToPrimaryValue = this->primary_plasma_current_PID->CalculatePID(inputstruct[0].PlasmaCurrent,inputstruct[0].PrimaryOutputWaveform);

					old_PrimaryWaveformMode = 2;
				}


				///////////////////// LQR  /////////////////////////////////////////////

				if (inputstruct[0].VerticalWaveformMode == 8 && inputstruct[0].HorizontalWaveformMode == 8){
                       /// Positive current
					if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 1000 && inputstruct[0].DischargeStatus==1) {
						this-> LQRcurrents= this->Kalman_LQR_var->MIMO_CONTROL_POSITIVE((inputstruct[0].VerticalOutputWaveform/1000),inputstruct[0].HorizontalOutputWaveform/1000, inputstruct[0].PositionR, inputstruct[0].PositionZ, inputstruct[0].VerticalCurrent, inputstruct[0].HorizontalCurrent);

						//this-> LQRcurrents= this->Kalman_LQR_var->MIMO_CONTROL_POSITIVE((inputstruct[0].VerticalOutputWaveform/1000),inputstruct[0].HorizontalOutputWaveform/1000, inputstruct[0].PositionR, inputstruct[0].PositionZ, SendToVer_buff, SendToHor_buff);

						this->SendToVer_buff = this->LQRcurrents.Ivert;
						this->SendToHor_buff =this->LQRcurrents.Ihor;


											}
					     /// Negative current
					if (inputstruct[0].PrimaryCurrent < 50 && inputstruct[0].PlasmaCurrent < -1000 && inputstruct[0].DischargeStatus==1) {
						this-> LQRcurrents= this->Kalman_LQR_var->MIMO_CONTROL_NEGATIVE((inputstruct[0].VerticalOutputWaveform/1000), inputstruct[0].HorizontalOutputWaveform/1000, inputstruct[0].PositionR, inputstruct[0].PositionZ, inputstruct[0].VerticalCurrent, inputstruct[0].HorizontalCurrent);

						//this-> LQRcurrents= this->Kalman_LQR_var->MIMO_CONTROL_NEGATIVE((inputstruct[0].VerticalOutputWaveform/1000), inputstruct[0].HorizontalOutputWaveform/1000, inputstruct[0].PositionR, inputstruct[0].PositionZ, SendToVer_buff, SendToHor_buff);

						this->SendToVer_buff =this->LQRcurrents.Ivert;
						this->SendToHor_buff =this->LQRcurrents.Ihor;


					}
					// else: keep the output (no changes)
					//// Limitações

						if ( this->SendToVer_buff > this->maximum_vertical_current ) this->SendToVer_buff = this->maximum_vertical_current;
						if ( this->SendToVer_buff < this->minimum_vertical_current ) this->SendToVer_buff = this->minimum_vertical_current;
						if ( this->SendToHor_buff > this->maximum_horizontal_current ) this->SendToHor_buff = this->maximum_horizontal_current;
						if ( this->SendToHor_buff < this->minimum_horizontal_current ) this->SendToHor_buff = this->minimum_horizontal_current;

					old_VerticalWaveformMode = 8;
					old_HorizontalWaveformMode = 8;
				}

				/////////////// This else is in case someone is selecting MIMO just for one of the currents (Vertical or Horizontal) .. PID will be execute for both
	 			else {
					if (inputstruct[0].VerticalWaveformMode == 2 || inputstruct[0].VerticalWaveformMode == 8){

						if(old_VerticalWaveformMode != 2) this->horizontal_position_PID->SetPIDConstants(this->PID_vertical_proportional_soft, this->PID_vertical_integral_soft, this->PID_vertical_derivative_soft, this->usecthread_cycle_time);

						if(old_VerticalWaveformMode > 5 || old_VerticalWaveformMode < 2 ) this->horizontal_position_PID->LoadOldOutputWithinLimits((inputstruct[0].VerticalCurrent));

						if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 850) {

							//this->SendToVer_buff = this->horizontal_position_PID->CalculatePID((2 *(inputstruct[0].VerticalOutputWaveform/1000) - inputstruct[0].PositionR),(inputstruct[0].VerticalOutputWaveform/1000));
							this->SendToVer_buff = this->horizontal_position_PID->CalculatePID_vert(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000),2,0);
						}
						if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -850) {
							this->SendToVer_buff = this->horizontal_position_PID->CalculatePID_vert(inputstruct[0].PositionR,(inputstruct[0].VerticalOutputWaveform/1000),1,0);
						}
						// else: keep the output (no changes)

						old_VerticalWaveformMode = 2;
					}
					if (inputstruct[0].HorizontalWaveformMode == 2 || inputstruct[0].HorizontalWaveformMode == 8){

						if(old_HorizontalWaveformMode != 2) this->vertical_position_PID->SetPIDConstants(this->PID_horizontal_proportional_soft, this->PID_horizontal_integral_soft, this->PID_horizontal_derivative_soft, this->usecthread_cycle_time);

						if(old_HorizontalWaveformMode > 5 || old_HorizontalWaveformMode < 2 ) this->vertical_position_PID->LoadOldOutputWithinLimits(inputstruct[0].HorizontalCurrent);

						if (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -900) {

							//outputstruct[0].SendToHorizontalValue = this->vertical_position_PID->CalculatePID((2 * inputstruct[0].HorizontalOutputWaveform/1000 - inputstruct[0].PositionZ), inputstruct[0].HorizontalOutputWaveform/1000);
							this->SendToHor_buff = this->vertical_position_PID->CalculatePID_hor( inputstruct[0].PositionZ, inputstruct[0].HorizontalOutputWaveform/1000,2,0);
						}
						if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 900) {
							this->SendToHor_buff = this->vertical_position_PID->CalculatePID_hor(inputstruct[0].PositionZ, inputstruct[0].HorizontalOutputWaveform/1000,1,0);
						}
						// else: keep the output (no changes)

						old_HorizontalWaveformMode = 2;
					}
				}




				/////////// Current control
				if (inputstruct[0].PrimaryWaveformMode == 1 || inputstruct[0].PrimaryWaveformMode == 8){

					outputstruct[0].SendToPrimaryValue = inputstruct[0].PrimaryOutputWaveform;

					old_PrimaryWaveformMode = 1;
				}
				if (inputstruct[0].VerticalWaveformMode == 1 ){

					this->SendToVer_buff = inputstruct[0].VerticalOutputWaveform;

					old_VerticalWaveformMode = 1;
				}
				if (inputstruct[0].HorizontalWaveformMode == 1 ){

					this->SendToHor_buff = inputstruct[0].HorizontalOutputWaveform;

					old_HorizontalWaveformMode = 1;
				}
				if (inputstruct[0].PrimaryWaveformMode == 0){

					outputstruct[0].SendToPrimaryValue = 0;
					old_PrimaryWaveformMode = 0;
				}
				if (inputstruct[0].VerticalWaveformMode == 0){

					this->SendToVer_buff = 0;
					old_VerticalWaveformMode = 0;
				}
				if (inputstruct[0].HorizontalWaveformMode == 0){

					this->SendToHor_buff = 0;
					old_HorizontalWaveformMode = 0;
				}
			}

		}
		else {			//GAM online but discharge status < 0
			this->SendToHor_buff = 0;
			this->SendToVer_buff = 0;
			outputstruct[0].SendToPrimaryValue = 0;
			outputstruct[0].SendToPuffing = 0;
			outputstruct[0].SendToToroidal = 0;
		}


		if (inputstruct[0].DischargeStatus == 0) outputstruct[0].SendToPuffing = (float) inputstruct[0].PuffingOutputWaveform;// discharge started, lasts until end of breakdown

		if (inputstruct[0].DischargeStatus > 0){ // inputstruct[0].DischargeStatus == 1 -> timewindows, inputstruct[0].DischargeStatus == 2 -> inversion

			if ( puffing_mode == 2 || puffing_mode == 4){ //puffing feedback

				if (old_DischargeStatus < 1){  // transition from breakdown to time-windows

					puffing_feedback_last_usectime = inputstruct[0].usecTime;
					puffing_feedback_usectime_to_change = minimum_idle_time_in_puffing_feedback_in_us;
					puffing_feedback_currently_off = True;
				}
				else {  // during time window operation

					if (puffing_feedback_currently_off) {

						if ( puffing_feedback_last_usectime + puffing_feedback_usectime_to_change < inputstruct[0].usecTime){

	// AssertErrorCondition(InitialisationError,"ControllerGAM::!!! RT info, apagar!!! puffing usec to change  = %d HAlpha = %f waveform = %f",puffing_feedback_usectime_to_change, inputstruct[0].HAlpha, inputstruct[0].PuffingOutputWaveform);
							puffing_feedback_last_usectime = inputstruct[0].usecTime;
							puffing_feedback_currently_off = False;
						}

						if (puffing_feedback_mode == 1 && ((inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) || (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent < -750))){  // feedback in density
							if ( inputstruct[0].PuffingOutputWaveform < inputstruct[0].Density) puffing_feedback_usectime_to_change = puffing_feedback_usectime_to_change + puffing_feedback_usec_change_per_cycle;
							if ( inputstruct[0].PuffingOutputWaveform > inputstruct[0].Density) puffing_feedback_usectime_to_change = puffing_feedback_usectime_to_change - puffing_feedback_usec_change_per_cycle;
							if ( puffing_feedback_usectime_to_change > maximum_idle_time_in_puffing_feedback_in_us ) puffing_feedback_usectime_to_change = maximum_idle_time_in_puffing_feedback_in_us;
							if ( puffing_feedback_usectime_to_change < minimum_idle_time_in_puffing_feedback_in_us ) puffing_feedback_usectime_to_change = minimum_idle_time_in_puffing_feedback_in_us;
						}
						if(puffing_feedback_mode == 2 && ((inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 750) || (inputstruct[0].PrimaryCurrent < -25 && inputstruct[0].PlasmaCurrent > -750))) {		// feedback in h-alpha
							if ( inputstruct[0].PuffingOutputWaveform < inputstruct[0].HAlpha) puffing_feedback_usectime_to_change = puffing_feedback_usectime_to_change + puffing_feedback_usec_change_per_cycle;
							if ( inputstruct[0].PuffingOutputWaveform > inputstruct[0].HAlpha) puffing_feedback_usectime_to_change = puffing_feedback_usectime_to_change - puffing_feedback_usec_change_per_cycle;
							if ( puffing_feedback_usectime_to_change > maximum_idle_time_in_puffing_feedback_in_us ) puffing_feedback_usectime_to_change = maximum_idle_time_in_puffing_feedback_in_us;
							if ( puffing_feedback_usectime_to_change < minimum_idle_time_in_puffing_feedback_in_us ) puffing_feedback_usectime_to_change = minimum_idle_time_in_puffing_feedback_in_us;
						}
					}
					else {
						if ( puffing_feedback_last_usectime + puffing_duration_in_puffing_feedback_in_us < inputstruct[0].usecTime){

							puffing_feedback_last_usectime = inputstruct[0].usecTime;
							puffing_feedback_currently_off = True;
						}
					}

				}

				if (puffing_feedback_currently_off) outputstruct[0].SendToPuffing = minimum_puffing_output; // turn puffing off
				if (!puffing_feedback_currently_off && inputstruct[0].PrimaryCurrent >= 45 ) outputstruct[0].SendToPuffing = maximum_puffing_output;
				if (!puffing_feedback_currently_off && inputstruct[0].PrimaryCurrent <= -45) outputstruct[0].SendToPuffing = maximum_puffing_output;
				if (inputstruct[0].PrimaryCurrent < 45 && inputstruct[0].PrimaryCurrent >-45)outputstruct[0].SendToPuffing = minimum_puffing_output;


			}
			if (puffing_mode == 1 || puffing_mode == 3) outputstruct[0].SendToPuffing = (float) inputstruct[0].PuffingOutputWaveform;

		}

		if ( puffing_mode == 0) outputstruct[0].SendToPuffing = 0;
//		if ( puffing_mode == 3) outputstruct[0].SendToPuffing = (float) inputstruct[0].PuffingOutputWaveform;
		outputstruct[0].SendToToroidal = (float) inputstruct[0].ToroidalOutputWaveform;
	}


/////////////////////////// Kalman Filtering //////////////////////////


if (inputstruct[0].PrimaryCurrent > 25 && inputstruct[0].PlasmaCurrent > 1000 && inputstruct[0].DischargeStatus==1 ) {
	//

	if(this->changeDetec == 0){
		int ivo;
		ivo = this->Kalman_LQR_var->erase(Radial_pos, Vertical_pos);
		this->changeDetec =1;
		}

	this-> CentroidPos= this->Kalman_LQR_var->KALMAN_FILTER_POS(Radial_pos, Vertical_pos,SendToVer_buff, SendToHor_buff, 1);
	//this-> CentroidPos= this->Kalman_LQR_var->KALMAN_FILTER_POS(Radial_pos, Vertical_pos,inputstruct[0].VerticalCurrent, inputstruct[0].HorizontalCurrent, 1);

	//this->CentroidPos.Kalman_R=0.05;
	//this->CentroidPos.Kalman_Z=0.05;



} else if ( inputstruct[0].PrimaryCurrent < 50 &&  inputstruct[0].PlasmaCurrent < -1000  && inputstruct[0].DischargeStatus==1 )  {
	//

	if(this->changeDetec == 0){
		int ivo;
		ivo = this->Kalman_LQR_var->erase(Radial_pos, Vertical_pos);
		this->changeDetec =1;
		}

	//this-> CentroidPos= this->Kalman_LQR_var->KALMAN_FILTER_NEG(Radial_pos, Vertical_pos, inputstruct[0].VerticalCurrent, inputstruct[0].HorizontalCurrent, 1);
	this-> CentroidPos= this->Kalman_LQR_var->KALMAN_FILTER_NEG(Radial_pos, Vertical_pos, SendToVer_buff, SendToHor_buff, 1);

	//this->CentroidPos.Kalman_R=0.05;
	//this->CentroidPos.Kalman_Z=0.05;



}
	else{
		this-> CentroidPos= this->Kalman_LQR_var->KALMAN_FILTER_NEG(0, 0, 0, 0, 0);
		this->changeDetec =0;
	}

		outputstruct[0].R_recons=this->CentroidPos.Kalman_R;
		outputstruct[0].Z_recons=this->CentroidPos.Kalman_Z;



///////////////////////////////////////////////////////////////////////////////

/////// SendTo Vertical e Horizontal

	 outputstruct[0].SendToVerticalValue = this->SendToVer_buff;
	 outputstruct[0].SendToHorizontalValue = this->SendToHor_buff;
/////////////////////

	old_DischargeStatus = inputstruct[0].DischargeStatus;

//	outputstruct[0].SendToVerticalValue = (float) inputstruct[0].PlasmaCurrent;
//	AssertErrorCondition(InitialisationError,"ControllerGAM:: %s outputstruct = %f %f %f %f",this->Name(), outputstruct[0].SendToHorizontalValue, outputstruct[0].SendToVerticalValue, outputstruct[0].SendToPrimaryValue, outputstruct[0].SendToPuffing);
	this->SignalsOutputInterface->Write();


	return True;
}
bool ControllerGAM::ProcessHttpMessage(HttpStream &hStream){

	HtmlStream hmStream(hStream);
	int i;
	hmStream.SSPrintf(HtmlTagStreamMode, "html>\n\
		<head>\n\
		<title>%s</title>\n\
		</head>\n\
		<body>\n\
		<svg width=\"100&#37;\" height=\"100\" style=\"background-color: AliceBlue;\">\n\
		<image x=\"%d\" y=\"%d\" width=\"%d\" height=\"%d\" xlink:href=\"%s\" />\n\
		</svg", (char *) this->Name() ,0, 0, 422, 87, "http://www.ipfn.ist.utl.pt/ipfnPortalLayout/themes/ipfn/_img_/logoIPFN_Topo_officialColours.png");

	hmStream.SSPrintf(HtmlTagStreamMode, "br><br><text style=\"font-family:Arial;font-size:46\">%s</text><br", (char *) this->Name());

    FString submit_view;
    submit_view.SetSize(0);
    if (hStream.Switch("InputCommands.submit_view")){
        hStream.Seek(0);
        hStream.GetToken(submit_view, "");
        hStream.Switch((uint32)0);
    }
	if(submit_view.Size() > 0) view_input_variables = True;

	FString submit_hide;
    submit_hide.SetSize(0);
    if (hStream.Switch("InputCommands.submit_hide")){
        hStream.Seek(0);
        hStream.GetToken(submit_hide, "");
        hStream.Switch((uint32)0);
    }
	if(submit_hide.Size() > 0) view_input_variables = False;

	hmStream.SSPrintf(HtmlTagStreamMode, "form enctype=\"multipart/form-data\" method=\"post\"");
	if(!view_input_variables){
		hmStream.SSPrintf(HtmlTagStreamMode, "input type=\"submit\" name=\"submit_view\" value=\"View input variables\"");
	}
	else {
		hmStream.SSPrintf(HtmlTagStreamMode, "input type=\"submit\" name=\"submit_hide\" value=\"Hide input variables\"");
		hmStream.SSPrintf(HtmlTagStreamMode, "br><br>interferometry_radial_control_bool = %d\n\
		<br>PID_horizontal_proportional_soft = %.2f\n\
		<br>PID_horizontal_proportional_normal = %.2f\n\
		<br>PID_horizontal_proportional_hard = %.2f\n\
		<br>PID_horizontal_integral_soft = %.2f\n\
		<br>PID_horizontal_integral_normal = %.2f\n\
		<br>PID_horizontal_integral_hard = %.2f\n\
		<br>PID_horizontal_derivative_soft = %.2f\n\
		<br>PID_horizontal_derivative_normal = %.2f\n\
		<br>PID_horizontal_derivative_hard = %.2f\n\
		<br>PID_vertical_proportional_soft = %.2f\n\
		<br>PID_vertical_proportional_normal = %.2f\n\
		<br>PID_vertical_proportional_hard = %.2f\n\
		<br>PID_vertical_integral_soft = %.2f\n\
		<br>PID_vertical_integral_normal = %.2f\n\
		<br>PID_vertical_integral_hard = %.2f\n\
		<br>PID_vertical_derivative_soft = %.2f\n\
		<br>PID_vertical_derivative_normal = %.2f\n\
		<br>PID_vertical_derivative_hard = %.2f\n\
		<br>PID_primary_proportional_soft = %.2f\n\
		<br>PID_primary_proportional_normal = %.2f\n\
		<br>PID_primary_proportional_hard = %.2f\n\
		<br>PID_primary_integral_soft = %.2f\n\
		<br>PID_primary_integral_normal = %.2f\n\
		<br>PID_primary_integral_hard = %.2f\n\
		<br>PID_primary_derivative_soft = %.2f\n\
		<br>PID_primary_derivative_normal = %.2f\n\
		<br>PID_primary_derivative_hard = %.2f\n\
		<br><br",interferometry_radial_control_bool,PID_horizontal_proportional_soft,PID_horizontal_proportional_normal,PID_horizontal_proportional_hard,PID_horizontal_integral_soft,PID_horizontal_integral_normal,PID_horizontal_integral_hard,PID_horizontal_derivative_soft,PID_horizontal_derivative_normal,PID_horizontal_derivative_hard,PID_vertical_proportional_soft,PID_vertical_proportional_normal,PID_vertical_proportional_hard,PID_vertical_integral_soft,PID_vertical_integral_normal,PID_vertical_integral_hard,PID_vertical_derivative_soft,PID_vertical_derivative_normal,PID_vertical_derivative_hard,PID_primary_proportional_soft,PID_primary_proportional_normal,PID_primary_proportional_hard,PID_primary_integral_soft,PID_primary_integral_normal,PID_primary_integral_hard,PID_primary_derivative_soft,PID_primary_derivative_normal,PID_primary_derivative_hard);

		int j;
		hmStream.SSPrintf(HtmlTagStreamMode, "br><b>A_matrix</b>\n<table border=\"1\"");
		for (j=0;j<A_matrix_dims[0];j++){
			hmStream.SSPrintf(HtmlTagStreamMode, "tr");
			for (i=0;i<A_matrix_dims[1];i++)hmStream.SSPrintf(HtmlTagStreamMode, "td> %f </td",A_matrix[j][i]);
			hmStream.SSPrintf(HtmlTagStreamMode, "/tr");
		}
		hmStream.SSPrintf(HtmlTagStreamMode, "/table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "br><b>B_matrix</b>\n<table border=\"1\"");
		for (j=0;j<B_matrix_dims[0];j++){
			hmStream.SSPrintf(HtmlTagStreamMode, "tr");
			for (i=0;i<B_matrix_dims[1];i++)hmStream.SSPrintf(HtmlTagStreamMode, "td> %f </td",B_matrix[j][i]);
			hmStream.SSPrintf(HtmlTagStreamMode, "/tr");
		}
		hmStream.SSPrintf(HtmlTagStreamMode, "/table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "br><b>C_matrix</b>\n<table border=\"1\"");
		for (j=0;j<C_matrix_dims[0];j++){
			hmStream.SSPrintf(HtmlTagStreamMode, "tr");
			for (i=0;i<C_matrix_dims[1];i++)hmStream.SSPrintf(HtmlTagStreamMode, "td> %f </td",C_matrix[j][i]);
			hmStream.SSPrintf(HtmlTagStreamMode, "/tr");
		}
		hmStream.SSPrintf(HtmlTagStreamMode, "/table><br");

		hmStream.SSPrintf(HtmlTagStreamMode, "br><b>D_matrix</b>\n<table border=\"1\"");
		for (j=0;j<D_matrix_dims[0];j++){
			hmStream.SSPrintf(HtmlTagStreamMode, "tr");
			for (i=0;i<D_matrix_dims[1];i++)hmStream.SSPrintf(HtmlTagStreamMode, "td> %f </td",D_matrix[j][i]);
			hmStream.SSPrintf(HtmlTagStreamMode, "/tr");
		}
		hmStream.SSPrintf(HtmlTagStreamMode, "/table><br");
	}
	hmStream.SSPrintf(HtmlTagStreamMode, "/form");

 	hmStream.SSPrintf(HtmlTagStreamMode, "/body>\n</html");
	hStream.SSPrintf("OutputHttpOtions.Content-Type","text/html;charset=utf-8");
	hStream.WriteReplyHeader(True);


	return True;
}
