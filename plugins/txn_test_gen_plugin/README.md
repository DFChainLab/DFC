# txn\_test\_gen\_plugin

This plugin provides a way to generate a given amount of transactions per second against the currency contract. It runs internally to dfcd to reduce overhead.

This general procedure was used when doing Dawn 3.0 performance testing as mentioned in https://github.com/FutureSharesCloud/dfc/issues/2078.

## Performance testing

The following instructions describe how to use the `txn_test_gen_plugin` plugin to generate 1,000 transaction per second load on a simple DFCIO node.

### Create config and data directories
Make an empty directory for our configs and data, `mkdir ~/dfc.data`, and define a logging.json that doesn't print debug information (which occurs for each txn) to the console:
```bash
cat << EOF > ~/dfc.data/logging.json
{
  "includes": [],
  "appenders": [{
      "name": "consoleout",
      "type": "console",
      "args": {
        "stream": "std_out",
        "level_colors": [{
            "level": "debug",
            "color": "green"
          },{
            "level": "warn",
            "color": "brown"
          },{
            "level": "error",
            "color": "red"
          }
        ]
      },
      "enabled": true
    }
  ],

  "loggers": [{
      "name": "default",
      "level": "info",
      "enabled": true,
      "additivity": false,
      "appenders": [
        "consoleout"
      ]
    }
  ]
}
EOF
```

### Launch producer
```bash
$ ./noddfc -d ~/dfc.data/producer_node --config-dir ~/dfc.data/producer_node -l ~/dfc.data/logging.json --http-server-address "" -p dfcio -e
```

### Launch non-producer that will generate transactions
```bash
$ ./noddfc -d ~/dfc.data/generator_node --config-dir ~/dfc.data/generator_node -l ~/dfc.data/logging.json --plugin dfcio::txn_test_gen_plugin --plugin dfcio::chain_api_plugin --p2p-peer-address localhost:9876 --p2p-listen-endpoint localhost:5555
```

### Create a wallet on the non-producer and set bios contract
```bash
$ ./cldfc wallet create --to-console
$ ./cldfc wallet import --private-key 5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3
$ ./cldfc set contract dfcio ~/dfc/build.release/contracts/dfcio.bios/ 
```

### Initialize the accounts txn_test_gen_plugin uses
```bash
$ curl --data-binary '["dfcio", "5KQwrPbwdL6PhXujxW37FSSQZ1JiwsST4cqQzDeyXtP79zkvFD3"]' http://127.0.0.1:8888/v1/txn_test_gen/create_test_accounts
```

### Start transaction generation, this will submit 20 transactions evey 20ms (total of 1000TPS)
```bash
$ curl --data-binary '["", 20, 20]' http://127.0.0.1:8888/v1/txn_test_gen/start_generation
```

### Note the producer console prints
```bash
dfcio generated block 9b8b851d... #3219 @ 2018-04-25T16:07:47.000 with 500 trxs, lib: 3218
dfcio generated block e5b3cd5d... #3220 @ 2018-04-25T16:07:47.500 with 500 trxs, lib: 3219
dfcio generated block b243aeaa... #3221 @ 2018-04-25T16:07:48.000 with 500 trxs, lib: 3220
```

Note in the console output there are 500 transactions in each of the blocks which are produced every 500 ms yielding 1,000 transactions / second.

### Demonstration
The following video provides a demo: https://vimeo.com/266585781
