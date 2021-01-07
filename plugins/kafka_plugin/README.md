# DFCIO Kafka Plugin
## what's dfcio kafka plugin
DFCIO Kafka Plugin is used to receive the transaction data fom blockchain and send out the transaction through kafka producer. Developer can receive the transaction data through kafka consumer in the background application.

## how does the kafka plugin work

1. it run a task to resume the transactions on chain. there's two type of transactions:"applied transaction" and "accepted transaction"

2. create two kafka topics, the producer of which store the applied transaction and accepted transaction in kafka queue

3. the dapp developer can get the transaction data through the consumer of the kafka topic.

## Based dfcio version
dfc.io-1.6.1 or later

## Building the plugin [Install on your noddfc server]

1. install kafka library
```
cd /usr/local
git clone https://github.com/edenhill/librdkafka.git
cd librdkafka
./configure
make
sudo make install
```
2. download the kafka plugin code in to dfc file
```
cd /usr/local/dfc/plugins/
git clone https://github.com/tokenbankteam/kafka_plugin.git
```
3. update the CMakeLists.txt to complie the kafka plugin 
```
(1)edit /usr/local/dfc/plugins/CMakeLists.txt:
add_subdirectory(kafka_plugin)

(2)edit /usr/local/dfc/programs/noddfc/CMakeLists.txt:
target_link_libraries( noddfc PRIVATE -Wl,${whole_archive_flag} kafka_plugin -Wl,${no_whole_archive_flag} )
```

## How to setup on your noddfc
Enable this plugin using --plugin option to noddfc or in your config.ini. Use noddfc --help to see options used by this plugin.

## Configuration
Add the following to config.ini to enable the plugin:
```
parmeters for kafka_plugin
 --plugin dfcio::kafka_plugin     //add kafka plugin
 --kafka-uri 192.168.31.225:9092    //the kafka service 
 --accept_trx_topic dfc_accept_topic //the kafka topic for accept transaction
 --applied_trx_topic dfc_applied_topic //the applied topic for applied transaction
 --kafka-block-start 100  //the start block number, from which kafka begin to receive transactions
 --kafka-queue-size 5000  // the queue size of kafka
```
