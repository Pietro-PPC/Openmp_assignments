N=$1

for i in $(seq 1 50)
do
  ./a.out $N 20 >> inps_${N}/inp_${i}.in
  sleep 1
done
