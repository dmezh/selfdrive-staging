#!/usr/bin/env python3
import argparse

import cand


def main():
    argparser = argparse.ArgumentParser(description='py-steering')
    argparser.add_argument(
        '--redis-host',
        default='localhost',
        help='redis hostname',
        metavar='hostname',
    )
    argparser.add_argument(
        '--redis-port',
        default='6379',
        help='redis port',
        metavar='6379',
    )

    args = argparser.parse_args()

    bus = cand.client.Bus(
        redis_host=args.redis_host,
        redis_port=args.redis_port,
    )


if __name__ == '__main__':
    main()
