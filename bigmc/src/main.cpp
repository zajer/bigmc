using namespace std;
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <set>

#include <bigmc.h>

global_config global_cfg;

void print_usage(char **argv) {
	fprintf(stderr,
	"Usage: %s [-hvVl] [-G <file>] [-m <max steps>] [-r <interval>] <modelfile>\n"
	"\t-h\t\tDisplay this help and exit\n"
	"\t-V\t\tPrint verbose output\n"
	"\t-VV\t\tPrint even more verbose output\n"
	"\t-G x\t\tOutput graphviz dot file to x\n"
	"\t-m x\t\tSpecify x maximum steps of graph unfolding (default: 1000)\n"
	"\t-l\t\tLocal check mode - do not build the reaction graph.\n"
	"\t-r x\t\tOutput statistics and graphs every x steps (default: 500)\n"
	"\t-v\t\tPrint version information and exit\n",
	argv[0]);
}

void print_version() {
	cout << "BigMC version " << VERSION << " (http://www.itu.dk/~gdpe/bigmc)\n";
	cout << "Copyright (c) 2011 Gian Perrone <gdpe at itu.dk>\n";
}

void config_read() {
	// We try to locate bigmc.conf in this order:
	// If BIGMC_HOME is set:
	// $BIGMC_HOME/conf/bigmc.conf
	// otherwise:
	// /usr/local/bigmc/conf/bigmc.conf
	// ./bigmc.conf
	// ./conf/bigmc.conf
	// fail

	FILE *fp = NULL;
	char *configfile = NULL;

	char *bigmc_home = getenv("BIGMC_HOME");
	if(bigmc_home == NULL) {
		cerr << "Warning: BIGMC_HOME environment variable is not set." << endl;

		fp = fopen("/usr/local/bigmc/conf/bigmc.conf", "r");
		configfile = "/usr/local/bigmc/conf/bigmc.conf";

		if(fp == NULL) {
			fp = fopen("./bigmc.conf", "r");
			configfile = "./bigmc.conf";
		}

		if(fp == NULL) {
			fp = fopen("./conf/bigmc.conf","r");
			configfile = "./conf/bigmc.conf";
		}

		if(fp == NULL) {
			cerr << "Error: failed to locate bigmc.conf.  Try setting BIGMC_HOME\n";
			exit(1);
		}

		global_cfg.bigmc_home = ".";
	} else {
		char buf[4096] = { 0 };
		sprintf(buf,"%s/conf/bigmc.conf", bigmc_home);
		fp = fopen(buf, "r");
		configfile = strdup(buf);

		if(fp == NULL) {
			cerr << "Error: failed to locate " << buf << endl;
			exit(1);
		}

		global_cfg.bigmc_home = bigmc_home;
	}

	int clineno = 0;
	char line[2048] = {0};
	while(fgets(line, 2048, fp) != NULL) {
		clineno++;

		if(line[0] == '#' || line[0] == '\n' || line[0] == '\r') continue;

		if(strncmp(line, "predicate", 9) == 0) {
			char *c = line+9;
			while(*c == ' ' || *c == '\t') c++;
			// First we get the symbol name
			char sname[256];
			int i = 0;
			while(*c != ' ' && *c != '\t' && *c != '\n' && *c != '\r' && *c != '\0') {
				sname[i++] = *(c++);
			}
			sname[i] = '\0';

			// Skip any whitespace
			while(*c == ' ' || *c == '\t') c++;

			// Then we get the filename
			char fname[768];
			i = 0;
			while(*c != ' ' && *c != '\t' && *c != '\n' && *c != '\r' &&
				 *c != '\0' && *c != '#') {
				fname[i++] = *(c++);
			}
			fname[i] = '\0';

			if(strlen(sname) == 0 || strlen(fname) == 0) {
				cout << "Error: " << configfile << ":" << clineno << 
					": invalid predicate declaration" << endl;
				exit(1);
			}

			string psname = string(sname);

			query_predicate::register_predicate(psname, 
				new predicate(psname, fname));
		} else {
			cout << "Error: " << configfile << ":" << clineno << 
					": invalid configuration option" << endl;
				exit(1);

		}
	}
}

int main(int argc, char**argv) {
	int c;

	// Set config defaults
	global_cfg.debug = false;
	global_cfg.verbose_level = 0;
	global_cfg.max_steps = 1000;
	global_cfg.report_interval = 500;
	global_cfg.graph_out = NULL; 
	global_cfg.check_local = false;
	
	while ((c = getopt (argc, argv, "hvVdm:G:r:l")) != -1)
		switch (c) {
		case 'h':
			print_usage(argv);
			return 0;
		case 'V':
			global_cfg.verbose_level++;
			break;
		case 'v':
			print_version();
			return 0;
		case 'd':
			global_cfg.debug = true;
			break;
		case 'G':
			global_cfg.graph_out = strdup(optarg);
			break;
		case 'm':
			global_cfg.max_steps = (unsigned long)atol(optarg);
			break;
		case 'r':
			global_cfg.report_interval = (unsigned long)atol(optarg);
			break;
		case 'l':
			global_cfg.check_local = true;
		case '?':
			if (isprint (optopt))
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
			else
				fprintf (stderr, "Unknown option character `\\x%x'.\n",
				optopt);
		default:
			abort ();
           	}

	config_read();

	int index;
	char *modelfile = NULL;

	for (index = optind; index < argc; index++) {
		modelfile = argv[index];
	}

	if(!modelfile) {
		parser::init(NULL);
	} else {
		FILE *fp = fopen(modelfile, "r");
		if(fp == NULL) {
			perror("Cannot read model file");
			return 1;
		} else
			fclose(fp);

		global_cfg.model_file = modelfile;
		parser::init(modelfile);
	}

	global_cfg.model_file = modelfile;
	
	parser::parse();

	parser::cleanup();

	return 0;
}
