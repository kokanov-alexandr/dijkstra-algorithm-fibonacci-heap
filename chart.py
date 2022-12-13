import matplotlib.pyplot as plt
COUNTER_TESTS = 22
   
naive = open("TestsInfo/naive_implement_dij.txt", 'r', encoding="utf-8")
embedded_pq = open("TestsInfo/embedded_priority_queue_dij.txt", 'r', encoding="utf-8")
fib_heap = open("TestsInfo/fib_heap_dij.txt", 'r', encoding="utf-8")
count_elems = open("TestsInfo/count_elements.txt", 'r', encoding="utf-8")

elems = []
naive_t = []
embedded_pq_t = []
fib_heap_t = []
for i in range(COUNTER_TESTS):
    n = count_elems.readline().split()[0]
    elems.append(int(n))
    naive_t.append(float(naive.readline().split()[0]))
    embedded_pq_t.append(float(embedded_pq.readline().split()[0]))
    fib_heap_t.append(float(fib_heap.readline().split()[0]))

plt.xscale('log')
plt.xlabel("Размер данных")
plt.ylabel("Время работы")
plt.plot(elems, naive_t, label='Наивная реализация', color="black", marker="o")
plt.plot(elems, embedded_pq_t, label='Встроенная приоритетная очередь', color="blue", marker="o")
plt.plot(elems, fib_heap_t, label='Фибоначчиева куча', color="red", marker="o")
plt.title("Алгоритм Дейкстры")
plt.legend()
plt.show()
raw_input()

