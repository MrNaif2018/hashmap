## Lab 8

### Build instructions

```bash
make
./main
```

To get group stats, run `./groupstats`. Requires `250502.csv` and `250503.csv` present in current directory.

To compare hashmap implementations, run `make runbench`

Hashes and test datasets are left there for pure convenience.

Sources:

- [Murmur3 hash](https://github.com/PeterScott/murmur3)
- [Google CityHash in C](https://github.com/nusov/cityhash-c)
- [Alexa top 1 million sites](http://s3.amazonaws.com/alexa-static/top-1m.csv.zip)
