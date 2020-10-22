

import threading

queue = []
queue_lock = threading.Lock()
queue_cond = threading.Condition(queue
