#!/bin/sh
cd /opt/dfcio/bin

if [ ! -d "/opt/dfcio/bin/data-dir" ]; then
    mkdir /opt/dfcio/bin/data-dir
fi

if [ -f '/opt/dfcio/bin/data-dir/config.ini' ]; then
    echo
  else
    cp /config.ini /opt/dfcio/bin/data-dir
fi

if [ -d '/opt/dfcio/bin/data-dir/contracts' ]; then
    echo
  else
    cp -r /contracts /opt/dfcio/bin/data-dir
fi

while :; do
    case $1 in
        --config-dir=?*)
            CONFIG_DIR=${1#*=}
            ;;
        *)
            break
    esac
    shift
done

if [ ! "$CONFIG_DIR" ]; then
    CONFIG_DIR="--config-dir=/opt/dfcio/bin/data-dir"
else
    CONFIG_DIR=""
fi

exec /opt/dfcio/bin/noddfc $CONFIG_DIR "$@"
