#!/bin/sh

sudo GR_SCHEDULER=STS nice -n -20 ./WISP_reader.py 2>&1 | tee output.log
sudo chown -R elvis:elvis ~/usrp_logs
