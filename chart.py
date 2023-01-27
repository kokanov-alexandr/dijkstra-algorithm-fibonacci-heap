import matplotlib.pyplot as plt
COUNTER_TESTS = 10
   
embedded_pq = open("tests-Info/embedded_priority_queue_dij.txt", 'r', encoding="utf-8")
fib_heap = open("tests-Info/fib_heap_dij.txt", 'r', encoding="utf-8")
count_elems = open("tests-Info/count_elements.txt", 'r', encoding="utf-8")

elems = []
embedded_pq_t = []
fib_heap_t = []
for i in range(1, COUNTER_TESTS + 1):
    n = count_elems.readline().split()[0]
    elems.append(int(n))
    embedded_pq_t.append(float(embedded_pq.readline().split()[0]))
    fib_heap_t.append(float(fib_heap.readline().split()[0]))

plt.xscale('log')
plt.xlabel("Количество рёбер")
plt.ylabel("Время работы (сек)")
plt.plot(elems, embedded_pq_t, label='Встроенная приоритетная очередь', color="blue", marker="o", linestyle = '--')
plt.plot(elems, fib_heap_t, label='Фибоначчиева куча', color="red", marker="o")
plt.title("Алгоритм Дейкстры")
plt.legend()
plt.show()

