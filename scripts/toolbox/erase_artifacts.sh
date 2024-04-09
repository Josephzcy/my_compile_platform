#!/bin/sh
project_id=3256
token="S-QkAsiaspcm1bfT3Hep"
start_job=41176
end_job=45345
for job_id in $(seq $start_job $end_job)
do
    echo "Remove on job ${job_id}"
    curl --request DELETE --header "PRIVATE-TOKEN:${token}" "https://git.minieye.tech/api/v4/projects/${project_id}/jobs/${job_id}/artifacts"
#	curl --request POST   --header "PRIVATE-TOKEN:${token}" "https://git.minieye.tech/api/v4/projects/${project_id}/jobs/${job_id}/erase"
done
