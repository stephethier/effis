#include "adios2.h"
#include <mpi.h>
#include <thread>
#include <chrono>
#include <iostream>

int main(int argc, char **argv)
{
	MPI_Comm comm;
	MPI_Init(&argc, &argv);
	MPI_Comm_dup(MPI_COMM_WORLD, &comm);

    //@effis-init comm=comm
	adios2::ADIOS adios(comm, adios2::DebugON);

    //@effis-begin "Jabberwocky"->"Jaberwocky"
	adios2::IO reader_io = adios.DeclareIO("Jabberwocky");
	adios2::Engine reader = reader_io.Open("Jabberwocky.bp", adios2::Mode::Read, comm);

	adios2::Variable<int> random;
	std::vector<int> data;

    while (true)
	{
		adios2::StepStatus status = reader.BeginStep(adios2::StepMode::Read, 10.0);

        if (status == adios2::StepStatus::NotReady)
		{
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            continue;
		}
		else if (status != adios2::StepStatus::OK)
		{
            break;
		}
		std::cout << reader.CurrentStep() << std::endl;
		random = reader_io.InquireVariable<int>("RandomInts");
		adios2::Dims shape = random.Shape();
		std::cout << shape[0] << " " << shape[1] << std::endl;
		//random.SetSelection({{0}, {shape[0]}});
		//reader.Get(random, );
        reader.EndStep();
	}

    reader.Close();
    //@effis-end

   //@effis-finalize
}
