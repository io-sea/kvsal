#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <errno.h>
#include <string.h>
#include <ini_config.h>
#include <iosea/m0common.h>

static struct collection_item *cfg_items;
#define CONF "/etc/iosea.d/kvsns.ini"

int main(int argc, char *argv[])
{
	int rc;
	struct collection_item *errors = NULL;

	if (argc != 2) {
		fprintf(stderr, "del <key>\n");
		exit(1);
	}

	rc = config_from_file("libkvsns", CONF, &cfg_items,
				INI_STOP_ON_ERROR, &errors);
	if (rc) {
		fprintf(stderr, "Can't load config rc=%d\n", rc);

		free_ini_config_errors(errors);
		return -rc;
	}

	rc = m0init(cfg_items);
	if (rc != 0) {
		fprintf(stderr, "Can't start m0store lib, rc=%d\n", rc);
		exit(1);
	}

	rc = m0kvs_del(argv[1], strlen(argv[1]));

	fprintf(stderr, "m0kv_del: rc = %d\n", rc);

	m0fini();

	return 0;
}
