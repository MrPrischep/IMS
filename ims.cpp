#include <iostream>
#include <simlib.h>
#include <string>
#include <string.h>

Facility Intermediate_hopper1("Intermediate hopper 1"); double tIntermediate_hopper1 = 2;
Facility Intermediate_hopper2("Intermediate hopper 2"); double tIntermediate_hopper2 = 2;
// Facility                                                 Facility time
Facility Dry_Mixer("Dry Mixer");                        double tDryMixer = 10;
// Facility failure chance
double fDryMixer = 0.0025;
// Facility repair time
double ftDryMixer = 40;
// Facility failure counter
int fDryMixer_counter = 0;
// Facility counter
int DryMixer_counter = 0;
// Facility failure coefficient
double fcDryMixer = 0;

Facility Fabric_preparetor("Fabric prepare");           double tFabric_prepare = 10;
Facility Fabric_inspector("Fabric inspection");         double tFabric_inspection = 8;
Facility Fabric_cleaner("Fabric cleaner");              double tFabric_cleaner = 12;
Facility Fabric_ironer("Fabric ironing");               double tFabric_ironing = 2;

Facility Dry_preparetor("Dry components prepare");      double tDry_prepare = 8;

Facility Paint_preparator("Paint prepare");             double tPaint_prepare = 5;
Facility Paint_grinder("Paint grinder");                double tPaint_grinder = 7;
Facility Paint_hopper("Paint hopper");                  double tPaint_hopper = 20;

Facility Liquid_preparator("Liquid preparator");        double tLiquid_preparator = 8;
Facility Liquid_measuring_tank("Liquid preparator");    double tLiquid_measuring_tank = 4;
Facility Liquid_hopper("Liquid preparator");            double tLiquid_hopper = 20;

Facility Mixer1("Mixer1");                              double tMixer1 = 8;
double fMixer1 = 0.0025;
double ftMixer1 = 90;
int fMixer1_counter = 0;
int Mixer1_counter = 0;
double fcMixer1 = 0;
Facility Mixer2("Mixer2");                              double tMixer2 = 8;
double fMixer2 = 0.0025;
double ftMixer2 = 90;
int fMixer2_counter = 0;
int Mixer2_counter = 0;
double fcMixer2 = 0;
Facility Transporter("Transporter");                    double tTransporter = 3;
Facility Padding_machine("Padding machine");            double tPadding_machine = 4;
double fPadding_machine = 0.0025;
double ftPadding_machine = 90;
int fPadding_machine_counter = 0;
int Padding_machine_counter = 0;
double fcPadding_machine = 0;
Facility Geller("Gelling agent");                       double tGeller = 4;
double fGeller = 0.0025;
double ftGeller = 240;
int fGeller_counter = 0;
int Geller_counter = 0;
double fcGeller = 0;
Facility Calenders("Calenders");                        double tCalenders = 1;

Facility Cutting_table("Cutting table");                double tCutting_table = 1;
Facility Final_prepare("Final_prepare");                double tFinal_prepare = 22;

int fabric = 0;
int paint = 0;
int dry = 0;
int liquid = 0;
int mix = 0;

int cycle = 0;
int first_year_cycle = 0;
int last_year_cycle = 0;

double years = 1;
int failure = false;
bool mixer1_failure = false;
bool mixer2_failure = false;
int current_year = 0;

class Final_stage : public Process {
    void Behavior() {
        // std::cout << "End Process\n";
        Seize(Final_prepare);
        Wait(tFinal_prepare);
        Release(Final_prepare);
        cycle++;
        // if (cycle >= 13) {
            // Stop();
        // }

        // if(Time >= 960) {
            // Stop();
        // }
    }
};

class Cutting : public Process {
    void Behavior() {
        // std::cout << "Cutting Activated\n";
        Seize(Cutting_table);
        Wait(tCutting_table);
        Release(Cutting_table);

        (new Final_stage)->Activate();
    }
};

class Calenders_process : public Process {
    void Behavior() {
        // std::cout << "Calenders_process Activated\n";
        Seize(Calenders);
        Wait(tCalenders);
        Release(Calenders);

        (new Cutting)->Activate();
    }
};
class Gelling : public Process {
    void Behavior() {
        // std::cout << "Gelling Activated\n";
        Seize(Geller);
        if (failure && Random() < (fGeller+current_year*fcGeller)) {
            fGeller_counter++;
            Wait(ftGeller);
        }
        Wait(tGeller);
        Geller_counter++;
        Release(Geller);
        (new Calenders_process)->Activate();
    }
};

class Padding : public Process {
    void Behavior() {
         // std::cout << "Padding Activated\n";
        Seize(Padding_machine);
        if (failure && Random() < (fPadding_machine+current_year*fcPadding_machine)) {
            Wait(ftPadding_machine);
            fPadding_machine_counter++;
        }
        Wait(tPadding_machine);
        Padding_machine_counter++;
        Release(Padding_machine);

        (new Gelling)->Activate();
    }
};

class Begin_padding : public Process {
    void Behavior() {
        // std::cout << "Begin_padding Activated\n";
        // std::cout << "mix :" << mix << "    fabric: " << fabric << std::endl;
        if (mix > 0 && fabric > 0) {
            mix--;
            fabric--;
            (new Padding)->Activate();
        }
    }
};

class Transportation : public Process {
    void Behavior() {
        // std::cout << "Transportation Activated\n";
        Seize(Transporter);
        Wait(tTransporter);
        Release(Transporter);

        (new Begin_padding)->Activate();
    }
};

class Mixing2 : public Process {
    void Behavior() {
        // std::cout << "Mixing2 Activated\n";
        Seize(Mixer2);
        if (failure && Random() < (fMixer2+current_year*fcMixer2)) {
            mixer2_failure = true;
            Wait(ftMixer2);
            mixer2_failure = false;
            fMixer2_counter++;
        }
        if (mixer1_failure) {
            Wait(tMixer2/2);
        } else {
            Wait(tMixer2);
        }
        Release(Mixer2);
        Mixer2_counter++;
        mix++;

        (new Transportation)->Activate();
    }
};

class Mixing1 : public Process {
    void Behavior() {
        // std::cout << "Mixing1 Activated\n";
        Seize(Mixer1);
        if (failure && Random() < (fMixer1+current_year*fcMixer1)) {
            mixer1_failure = true;
            Wait(ftMixer1);
            mixer1_failure = false;
            fMixer1_counter++;
        }
        if (mixer2_failure) {
            Wait(tMixer1/2);
        } else {
            Wait(tMixer1);
        }
        Mixer1_counter++;
        Release(Mixer1);
        mix++;

        (new Transportation)->Activate();
    }
};

class Begin_mixing : public Process {
    void Behavior() {
        // std::cout << "Begin_mixing Activated\n";
        // std::cout << "paint: " << paint << "    dry: " << dry << "     liquid: " << liquid << std::endl;
        if (paint > 0 && dry > 0 && liquid > 0) {
            paint--;
            dry--;
            liquid--;
            (new Mixing1)->Activate();
            (new Mixing2)->Activate();
        }
    }
};

class Liquid_store : public Process {
    void Behavior() {
        // std::cout << "Liquid_store Activated\n";
        Seize(Liquid_hopper);
        Wait(tLiquid_hopper);
        Release(Liquid_hopper);

        (new Begin_mixing)->Activate();
    }
};

class Liquid_measuring : public Process {
    void Behavior() {
        // std::cout << "Liquid_measuring Activated\n";
        Seize(Liquid_measuring_tank);
        Wait(tLiquid_measuring_tank);
        Release(Liquid_measuring_tank);

        (new Liquid_store)->Activate();
    }
};

class Liquid_preparation : public Process {
    void Behavior() {
        // std::cout << "Liquid_preparation Activated\n";
        Seize(Liquid_preparator);
        Wait(tLiquid_preparator);
        Release(Liquid_preparator);
        liquid++;

        (new Liquid_measuring)->Activate();
    }
};

class Dry_Intermediate2 : public Process {
    void Behavior() {
        // std::cout << "Dry_Intermediate2 Activated\n";
        Seize(Intermediate_hopper2);
        Wait(tIntermediate_hopper2);
        Release(Intermediate_hopper2);

        (new Begin_mixing)->Activate();
    }
};

class Dry_Mixing : public Process {
    void Behavior() {
        // std::cout << "Dry_Mixing Activated\n";
        Seize(Dry_Mixer);
        if (failure && Random() < (fDryMixer+current_year*fcDryMixer)) {
            Wait(ftDryMixer);
            fDryMixer_counter++;
        }
        Wait(tDryMixer);
        DryMixer_counter++;
        Release(Dry_Mixer);

        (new Dry_Intermediate2)->Activate();
    }
};

class Dry_Intermediate1 : public Process {
    void Behavior() {
        // std::cout << "Dry_Intermediate1 Activated\n";
        Seize(Intermediate_hopper1);
        Wait(tIntermediate_hopper1);
        Release(Intermediate_hopper1);

        (new Dry_Mixing)->Activate();
    }
};

class Paint_store : public Process {
    void Behavior() {
        // std::cout << "Paint_store Activated\n";
        Seize(Paint_hopper);
        Wait(tPaint_hopper);
        Release(Paint_hopper);

        (new Begin_mixing)->Activate();
    }
};

class Paint_grinding : public Process {
    void Behavior() {
        // std::cout << "Paint_grinding Activated\n";
        Seize(Paint_grinder);
        Wait(tPaint_grinder);
        Release(Paint_grinder);

        (new Paint_store)->Activate();
    }
};

class Paint_preparation : public Process {
    void Behavior() {
        // std::cout << "Paint_preparation Activated\n";

        Seize(Paint_preparator);
        Wait(tPaint_prepare);
        Release(Paint_preparator);
        paint++;

        (new Paint_grinding)->Activate();
    }
};

class Dry_preparation : public Process {
    void Behavior() {
        // std::cout << "Dry_preparation Activated\n";

        Seize(Dry_preparetor);
        Wait(tDry_prepare);
        Release(Dry_preparetor);
        dry++;

        (new Paint_preparation)->Activate();
        (new Dry_Intermediate1)->Activate();
    }
};

class Fabric_ironing : public Process {
    void Behavior() {
        // std::cout << "Fabric_ironing Activated\n";

        Seize(Fabric_ironer);
        Wait(tFabric_ironing);
        Release(Fabric_ironer);

        (new Begin_padding)->Activate();
    }
};

class Fabric_cleaning : public Process {
    void Behavior() {
        // std::cout << "Fabric_cleaning Activated\n";

        Seize(Fabric_cleaner);
        Wait(tFabric_cleaner);
        Release(Fabric_cleaner);

        (new Fabric_ironing)->Activate();
    }
};

class Fabric_inspection : public Process {
    void Behavior() {
        // std::cout << "Fabric_inspection Activated\n";
        Seize(Fabric_inspector);
        Wait(tFabric_inspection);
        Release(Fabric_inspector);

        (new Fabric_cleaning)->Activate();
    }
};

class Fabric_preparation : public Process {
    void Behavior() {
        //  std::cout << "Fabric_preparation Activated\n";

        Seize(Fabric_preparetor);
        Wait(tFabric_prepare);
        Release(Fabric_preparetor);
        fabric++;

        (new Fabric_inspection)->Activate();
    }
};

class Materials_avaliable : public Process {
    void Behavior() {
        //  std::cout << "Materials_avaliable created\n";
        current_year = (int) Time/(1440 * 240);
        if(current_year == 1 && first_year_cycle == 0) {
            // std::cout << cycle << std::endl;
            first_year_cycle = cycle;
        } else if (current_year == years-1 && last_year_cycle == 0) {
            // std::cout << cycle << std::endl;
            last_year_cycle = cycle;
        }
        Wait(70);
        (new Materials_avaliable)->Activate();
        (new Fabric_preparation)->Activate();
        (new Dry_preparation)->Activate();
        (new Liquid_preparation)->Activate();
    }
};

void parse_args(int argc, char** argv, double* years, int* failures) {
    if (argc == 1) {
        *years = 1;
        *failures = false;
    } else if (argc == 2 &&  !strcmp(argv[1], "--help")) {
        std::cout << "Primary usage: `make run f=<value> y=<value> gf=<value> pf=<value> gf=<value> pf=<value> df=<value> m1f=<value> m2f=<value>" << std::endl;
        std::cout << "in that case all parameters are optional, default values are in doc p4.1" << std::endl;
        std::cout << "Usage: `./ims -f <value> -y <value> -gf <value> -pf <value> -gf <value> -pf <value> -df <value> -m1f <value> -m2f <value>" << std::endl;
        std::cout << "-f : failure flag, <f_value> = 0 -failures off, other-failure on" << std::endl;
        std::cout << "-y : number of years flag, <y_value> = n of working years for the factory" << std::endl;
        std::cout << "-gf : percent of geller years failure flag, <fc_value> = % of added failure chance annually" << std::endl;
        std::cout << "-pf : percent of Padding machine years failure flag, <fc_value> = % of added failure chance annually" << std::endl;
        std::cout << "-df : percent of Dry mixer years failure flag, <fc_value> = % of added failure chance annually" << std::endl;
        std::cout << "-m1f : percent of mixer 1 years failure flag, <fc_value> = % of added failure chance annually" << std::endl;
        std::cout << "-m2f : percent of mixer 2 years failure flag, <fc_value> = % of added failure chance annually" << std::endl;
        exit(0);
    } else if (argc == 15 && !strcmp(argv[1], "-f")  && !strcmp(argv[3], "-y")  && !strcmp(argv[5], "-gf") && !strcmp(argv[7], "-pf") &&
                             !strcmp(argv[9], "-df") && !strcmp(argv[11], "-m1f") && !strcmp(argv[13], "-m2f")) {
        try {
            *failures = std::stoi(argv[2]);
            *years = std::stod(argv[4]);
            fcGeller = std::stod(argv[6]);
            fcGeller/=100;
            fcPadding_machine = std::stod(argv[8]);
            fcPadding_machine/=100;
            fcDryMixer = std::stod(argv[10]);
            fcDryMixer/=100;
            fcMixer1 = std::stod(argv[12]);
            fcMixer1/=100;
            fcMixer2 = std::stod(argv[14]);
            fcMixer2/=100;
        }
        catch(const std::exception& e){
            std::cerr << e.what() << '\n';
        }
        
    } else {
        std::cout << "if you want to use simulation parameters, please, run `./ims --help`" << std::endl;
        exit(0);
    }
}

int main(int argc, char** argv) {
    parse_args(argc, argv, &years, &failure);
    
    Init(0, 1440 * 240 * years);
    std::cout << "Init> number of years: " << years << "  failures flag: " << std::endl;

    (new Materials_avaliable)->Activate();

    Run();

    SIMLIB_statistics.Output();
    std::cout << std::endl;
    std::cout << "made full product cycles: " << cycle << std::endl;
    std::cout << std::endl;
    std::cout << "Were produced: " << cycle*415 <<" m2" << std::endl;
    std::cout << std::endl;
    std::cout << "for " << cycle*415 <<" m2 of our factory output were processed:" << std::endl;
    double mass_output = cycle*415*0.004*1100;
    std::cout << ">>Polyvinyl chloride:  " << mass_output* 0.34 << " kg" << std::endl;
    std::cout << ">>Talc:  " << mass_output* 0.45 << " kg" << std::endl;
    std::cout << ">>Plasticizer:  " << mass_output* 0.2 << " kg" << std::endl;
    std::cout << ">>Barvivo:  " << mass_output* 0.01 << " kg" <<  std::endl;
    std::cout << ">>Textile:  " << (cycle*415)/4 << " m" << std::endl;
    std::cout << std::endl;
    std::cout << std::endl;
    if (failure) {
        std::cout << "faluires:" << std::endl;
        std::cout << ">>Geller            " << "number of operations: "<< Geller_counter << "   number of failures: "<< fGeller_counter << "  failure percent " << (fGeller_counter*100.0)/(fGeller_counter + Geller_counter) << "%" << std::endl;
        std::cout << ">>Padding machine   " << "number of operations: "<< Padding_machine_counter << "   number of failures: "<< fPadding_machine_counter << "  failure percent " << (fPadding_machine_counter*100.0)/(fPadding_machine_counter + Padding_machine_counter) << "%" << std::endl;
        std::cout << ">>DryMixer          " << "number of operations: "<< DryMixer_counter << "   number of failures: "<< fDryMixer_counter << "  failure percent " << (fDryMixer_counter*100.0)/(fDryMixer_counter + DryMixer_counter) << "%" << std::endl;
        std::cout << ">>Mixer 1           " << "number of operations: "<< Mixer1_counter << "   number of failures: "<< fMixer1_counter << "  failure percent " << (fMixer1_counter*100.0)/(fMixer1_counter + Mixer1_counter) << "%" << std::endl;
        std::cout << ">>Mixer 2           " << "number of operations: "<< Mixer2_counter << "   number of failures: "<< fMixer2_counter << "  failure percent " << (fMixer2_counter*100.0)/(fMixer2_counter + Mixer2_counter) << "%" << std::endl;
        if (years > 1) {
            std::cout << "  " << std::endl;
            std::cout << "1st year were produced: " << first_year_cycle*415 << "m2  last year were produced: " << (cycle-last_year_cycle)*415 << "m2  decrised in percente:" << 100 - ((cycle-last_year_cycle)*415*100.0)/(first_year_cycle*415) << "%"<< std::endl;
        }
    }
    return 0;
}