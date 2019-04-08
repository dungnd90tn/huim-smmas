export LD_LIBRARY_PATH=/usr/local/lib:/usr/lib:/usr/local/lib64:/usr/lib64

for x in {1..20} ; do
./HUIMSMMAS --file=DB/DB_chess.txt --threshold=0.25 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_chess.txt --threshold=0.255 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_chess.txt --threshold=0.26 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
done

for x in {1..20} ; do
./HUIMSMMAS --file=DB/DB_foodmart.txt --threshold=0.0011 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_foodmart.txt --threshold=0.0012 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_foodmart.txt --threshold=0.0013 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
done

for x in {1..20} ; do
./HUIMSMMAS --file=DB/DB_mushroom.txt --threshold=0.14 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_mushroom.txt --threshold=0.1425 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_mushroom.txt --threshold=0.145 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
done

for x in {1..20} ; do
./HUIMSMMAS --file=DB/DB_retail.txt --threshold=0.003 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_retail.txt --threshold=0.004 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
./HUIMSMMAS --file=DB/DB_retail.txt --threshold=0.005 --max=3000 --size=20 --pheromone=1 --rho=0.05 --alpha=1 --beta=2 --q0=0.88 --interval=100 --write
done
