"""Compile and exercise timer host policy with the common resource stage."""
from tools.script_recovery.oakvale_timer_host_checks import run
from tools.script_recovery.prepare_oakvale_timer_integration import prepare


if __name__ == '__main__':
    print(run(prepare_fn=prepare)['compiledTest'])
