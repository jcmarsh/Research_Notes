import argparse

parser = argparse.ArgumentParser(description='Process some integers.')

dut_settings = parser.add_argument_group('DUT settings')
dut_settings.add_argument(
        '--serial',
        metavar='PORT',
        dest='dut_serial_port',
        help='serial port (overridden by Simics)')

args = parser.parse_args()

print(args.dut_serial_port)
