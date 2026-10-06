/*
 * ЕДИНЫЙ ФАЙЛ СО ВСЕМИ ЛАБОРАТОРНЫМИ РАБОТАМИ
 *
 * Инструкция по использованию:
 * Чтобы скомпилировать и запустить определенную программу, раскомментируйте 
 * соответствующий #define (уберите // перед ним), а остальные оставьте закомментированными.
 */

// Выберите ОДНУ программу для запуска:
#define RUN_LAB_1_EVENTS_FUNC             // lab1.cpp 
//#define RUN_LAB_2_UNIFORM_QUEUE         // lab2.cpp 
//#define RUN_LAB_3_1_POISSON_STREAMS     // lab3.1.cpp
//#define RUN_LAB_3_MM1_TASK_STRUCT       // lab3.cpp 
//#define RUN_COMP_LAB_1_SORT             // CompModelLab1.cpp
//#define RUN_COMP_LAB_2_UNIFORM          // Comp_vodel_lab_2.cpp
//#define RUN_COMP_LAB_3_EXP              // Comp_model_lab_3.cpp
//#define RUN_COMP_LAB_4_SUPERPOSITION    // Comp_lab_4.cpp
//#define RUN_COMP_LAB_5_MULTIPHASE       // Comp_model_lab_5.cpp
//#define RUN_UNNAMED_1_INTERVALS         // Безымянный код 1 (интервалы, векторы streams)
//#define RUN_UNNAMED_2_QUEUE_SIMPLE      // Безымянный код 2 (простая очередь tau/sigma)
//#define RUN_UNNAMED_3_QUEUE_WAIT        // Безымянный код 3 (очередь M/M/1, F(x), ожидание)


// =====================================================================
// 1. lab1.cpp
// =====================================================================
#ifdef RUN_LAB_1_EVENTS_FUNC
#include <functional>
#include <iostream>
#include <map>
#include <random>

using namespace std;

void function1() {
    cout << "выполнена функция 1" << endl;
}

void function2() {
    cout << "выполнена функция 2" << endl;
}

void function3() {
    cout << "выполнена функция 3" << endl;
}

void function4() {
    cout << "выполнена функция 4" << endl;
}

int main() {
    map<double, int> events;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> timeDistribution(0.0, 100.0);
    uniform_int_distribution<int> functionDistribution(1, 4);

    while (events.size() < 100) {
        double time = timeDistribution(gen);
        int functionType = functionDistribution(gen);
        events.emplace(time, functionType);
    }

    const function<void()> functions[] = {
        function1,
        function2,
        function3,
        function4,
    };

    for (const auto& [time, functionType] : events) {
        cout << "Время: " << time << " -> ";
        functions[functionType - 1]();
    }

    return 0;
}
#endif


// =====================================================================
// 2. lab2.cpp
// =====================================================================
#ifdef RUN_LAB_2_UNIFORM_QUEUE
#include <algorithm>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>

using namespace std;

int main() {

    mt19937 mt(42);
    uniform_real_distribution<> tau(5.0, 7.0);
    uniform_real_distribution<> sigma(4.0, 10.0);

    int K = 0;       
    int TYP = 1;     
    int L = 0;       
    int N = 1000000; 

    int inCount = 0;
    int outCount = 0;
    int maxQueue = 0;
    double ML = 0.0;
    double systemtime = 0.0;
    double T1;
    double T2;

    const double INF = numeric_limits<double>::infinity();

    T1 = tau(mt);
    T2 = INF;

    cout << fixed << setprecision(6);

    while (inCount < N || K == 1 || L > 0) {
        systemtime = min(T1, T2);

        if (T1 <= T2) {
            TYP = 1;
        } else {
            TYP = 2;
        }

        if (TYP == 1) {
            inCount++;
            ML += L;

            if (K == 0) {
                K = 1;
                T2 = systemtime + sigma(mt);
            } else {
                L++;
                if (L > maxQueue) {
                    maxQueue = L;
                }
            }

            if (inCount < N) {
                double arrivalInterval = tau(mt);
                T1 = systemtime + arrivalInterval;
            } else {
                T1 = INF;
            }
        } else {
            outCount++;

            if (L > 0) {
                L--;
                double processingTime = sigma(mt);
                T2 = systemtime + processingTime;
            } else {
                K = 0;
                T2 = INF;
            }
        }

        cout << "Время: " << systemtime
                 << " | " << (TYP == 1 ? "IN" : "OUT")
                 << " | K = " << K
                 << " | очередь L = " << L << endl;
    }

    return 0;
}
#endif


// =====================================================================
// 3. lab3.1.cpp
// =====================================================================
#ifdef RUN_LAB_3_1_POISSON_STREAMS
#include <iostream>
#include <vector>
#include <random>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <numeric>

using namespace std;

int main() {
    const double T = 10000.0;
    const int bins = 10;

    int N;
    double a, b;

    cout << "Введите количество потоков N: ";
    cin >> N;
    cout << "Введите a: ";
    cin >> a;
    cout << "Введите b: ";
    cin >> b;

    if (N <= 0 || a >= b) {
        cout << "Ошибка: N должно быть больше 0, а b должно быть больше a.\n";
        return 1;
    }

    mt19937 generator(12345);
    uniform_real_distribution<double> randomInterval(a, b);

    cout << fixed << setprecision(6);
    cout << "a = " << a << ", b = " << b << ", T = " << T << "\n";
    cout << "\n";

    vector<double> events;

    for (int stream = 0; stream < N; ++stream) {
        double time = 0.0;

        while (time <= T) {
            time += randomInterval(generator);
            if (time <= T) {
                events.push_back(time);
            }
        }
    }

    sort(events.begin(), events.end());

    vector<double> intervals;
    for (size_t i = 1; i < events.size(); ++i) {
        intervals.push_back(events[i] - events[i - 1]);
    }

    double meanIntervalOneStream = (a + b) / 2.0;
    double lambda = N / meanIntervalOneStream;
    double maxInterval = 5.0 / lambda;
    double binWidth = maxInterval / bins;
    vector<int> counts(bins, 0);

    for (double interval : intervals) {
        if (interval < maxInterval) {
            int bin = static_cast<int>(interval / binWidth);
            counts[bin]++;
        }
    }

    cout << "Параметры: N = " << N << ", a = " << a
         << ", b = " << b << ", lambda = " << lambda << "\n";
    cout << "Плотность интервалов между соседними событиями\n";
    cout << "      Интервал            Число           Плотность эксп.  Плотность теор.\n";

    for (int i = 0; i < bins; ++i) {
        double left = i * binWidth;
        double right = (i + 1) * binWidth;
        double experimentalDensity = counts[i] / (intervals.size() * binWidth);
        double middle = (left + right) / 2.0;
        double theoreticalDensity = lambda * exp(-lambda * middle);

        cout << left << " - " << right << "    "
             << setw(8) << counts[i] << "    "
             << setw(14) << experimentalDensity << "    "
             << theoreticalDensity << "\n";
    }

    cout << "Средний интервал: "
         << (1.0 / lambda) << " теоретически, "
         << (intervals.empty() ? 0.0 : accumulate(intervals.begin(), intervals.end(), 0.0) / intervals.size())
         << " экспериментально\n";

    return 0;
}
#endif


// =====================================================================
// 4. lab3.cpp
// =====================================================================
#ifdef RUN_LAB_3_MM1_TASK_STRUCT
#include <algorithm>
#include <array>
#include <iomanip>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

using namespace std;

struct Task {
	int id;
	double arrivalTime;
	double processingTime;
};

int main() {
	constexpr double modelTime = 10000000.0;
	constexpr int bucketCount = 11;
	constexpr double lambda = 0.15;
	constexpr double mu = 0.30;

	random_device rd;
	mt19937 generator(rd());
	exponential_distribution<double> arrivalInterval(lambda);
	exponential_distribution<double> processingTime(mu);

	vector<Task> waitingTasks;
	array<int, bucketCount> waitingTimeBuckets{};

	double nextArrival = arrivalInterval(generator);
	double completionTime = 0.0;
	double totalWaitingTime = 0.0;
	double maximumWaitingTime = 0.0;
	int arrivedTasks = 0;
	int completedTasks = 0;
	int nextTaskId = 1;
	bool serverBusy = false;

	cout << fixed << setprecision(3);


	auto startTask = [&](const Task& task, double time) {
		double waitingTime = time - task.arrivalTime;
		int bucket = min(bucketCount - 1, static_cast<int>(waitingTime));
		waitingTimeBuckets[bucket]++;
		totalWaitingTime += waitingTime;
		maximumWaitingTime = max(maximumWaitingTime, waitingTime);
		completionTime = time + task.processingTime;
		serverBusy = true;
	};

	while (nextArrival <= modelTime || serverBusy) {
		bool arrivalEvent = !serverBusy || nextArrival <= completionTime;
		double systemTime;

		if (arrivalEvent && nextArrival <= modelTime) {
			systemTime = nextArrival;
			Task task{nextTaskId++, systemTime, processingTime(generator)};
			arrivedTasks++;

			if (serverBusy) {
				waitingTasks.push_back(task);
			} else {
				startTask(task, systemTime);
			}

			nextArrival = systemTime + arrivalInterval(generator);
		} else {
			systemTime = completionTime;
			completedTasks++;

			if (waitingTasks.empty()) {
				serverBusy = false;
			} else {
				uniform_int_distribution<size_t> index(0, waitingTasks.size() - 1);
				size_t selected = index(generator);
				Task task = waitingTasks[selected];
				waitingTasks[selected] = waitingTasks.back();
				waitingTasks.pop_back();
				startTask(task, systemTime);
			}
		}
	}

	cout << "Обработано задач: " << completedTasks << '\n';
	cout << "Поступило задач: " << arrivedTasks << '\n';
	cout << "Интенсивность поступления lambda = " << lambda << '\n';
	cout << "Интенсивность обслуживания mu = " << mu << '\n';
	cout << "Среднее время ожидания: " << totalWaitingTime / arrivedTasks << " сек.\n";
	cout << "Максимальное время ожидания: " << maximumWaitingTime << " сек.\n\n";
	cout << "Распределение и функция распределения времени ожидания:\n";
	cout << "Интервал | Число задач | Вероятность | F(x)\n";
	double cumulativeProbability = 0.0;

	for (int bucket = 0; bucket < bucketCount - 1; bucket++) {
		double probability = static_cast<double>(waitingTimeBuckets[bucket]) / arrivedTasks;
		cumulativeProbability += probability;
		cout << bucket << "-" << bucket + 1 << " сек. | "
			 << waitingTimeBuckets[bucket] << " | "
			 << probability << " | " << cumulativeProbability << '\n';
	}

	double probability = static_cast<double>(waitingTimeBuckets.back()) / arrivedTasks;
	cumulativeProbability += probability;
	cout << "10+ сек. | " << waitingTimeBuckets.back() << " | "
		 << probability << " | " << cumulativeProbability << '\n';

	return 0;
}
#endif


// =====================================================================
// 5. CompModelLab1.cpp
// =====================================================================
#ifdef RUN_COMP_LAB_1_SORT
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <clocale>

using namespace std;
using namespace chrono;

struct Event {
    double time;
    string type;

    bool operator<(const Event& other) const {
        return time < other.time;
    }
};

int main() {
    setlocale(LC_ALL, "");

    vector<Event> events;
    int num_requests = 5;

    mt19937 gen{ static_cast<mt19937::result_type>(
        steady_clock::now().time_since_epoch().count()
    ) };

    uniform_real_distribution<double> time_dist(0.0, 50.0);
    uniform_real_distribution<double> duration_dist(1.0, 10.0);

    for (int i = 0; i < num_requests; ++i) {
        double time_in = time_dist(gen);
        double time_out = time_in + duration_dist(gen);

        events.push_back({ time_in, "in" });
        events.push_back({ time_out, "out" });
    }

    sort(events.begin(), events.end());

    cout << fixed << setprecision(2);

    int current_in_system = 0;

    for (const auto& ev : events) {
        if (ev.type == "in") {
            current_in_system++;
        }
        else {
            current_in_system--;
        }

        cout << "Время: " << ev.time
            << " Событие: " << ev.type
            << " Запросов на сервере: " << current_in_system << "\n";
    }

    return 0;
}
#endif


// =====================================================================
// 6. Comp_vodel_lab_2.cpp
// =====================================================================
#ifdef RUN_COMP_LAB_2_UNIFORM
#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    random_device rd;
    mt19937 mt(rd());
    uniform_real_distribution<double> arrival_dist(1.0, 7.0);
    uniform_real_distribution<double> service_dist(1.0, 6.0);

    const double INF = numeric_limits<double>::infinity();

    const int max_customers = 10;
    int customer_index = 1;

    bool is_server_busy = false;
    int queue_length = 0;
    double total_queue_length = 0.0;

    double next_arrival = arrival_dist(mt);
    double next_departure = next_arrival + service_dist(mt);

    while (customer_index <= max_customers) {
        double current_time = min(next_arrival, next_departure);
        bool is_arrival = (next_arrival <= next_departure);

        cout << "Состояние (Сервер занят = " << is_server_busy
            << " Очередь = " << queue_length
            << ") t = " << current_time;

        if (is_arrival) {
            cout << "; Вход: i = " << customer_index << " (Прибытие)\n";
            total_queue_length += queue_length;

            if (customer_index < max_customers) {
                next_arrival = current_time + arrival_dist(mt);
            }
            else {
                next_arrival = INF;
            }
            customer_index++;

            if (!is_server_busy) {
                is_server_busy = true;
                next_departure = current_time + service_dist(mt);
            }
            else {
                queue_length++;
            }
        }
        else {
            cout << "; Выход (Уход)\n";

            if (queue_length == 0) {
                is_server_busy = false;
                next_departure = INF;
            }
            else {
                queue_length--;
                next_departure = current_time + service_dist(mt);
            }
        }
    }

    cout << "Средняя длина очереди: " << total_queue_length / max_customers << "\n";

    return 0;
}
#endif


// =====================================================================
// 7. Comp_model_lab_3.cpp
// =====================================================================
#ifdef RUN_COMP_LAB_3_EXP
#include <iostream>
#include <random>
#include <chrono>
#include <limits>
#include <algorithm>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    random_device rd;
    mt19937 mt(rd());

    exponential_distribution<double> arrival_dist(0.25);
    exponential_distribution<double> service_dist(0.3);

    const double INF = numeric_limits<double>::infinity();

    const int max_customers = 10;
    int customer_index = 1;

    bool is_server_busy = false;
    int queue_length = 0;
    double total_queue_length = 0.0;

    double next_arrival = arrival_dist(mt);
    double next_departure = next_arrival + service_dist(mt);

    while (customer_index <= max_customers) {
        double current_time = min(next_arrival, next_departure);
        bool is_arrival = (next_arrival <= next_departure);

        cout << "Состояние (Сервер занят = " << is_server_busy
            << " Очередь = " << queue_length
            << ") t = " << current_time;

        if (is_arrival) {
            cout << "; Вход: i = " << customer_index << " (Прибытие)\n";
            total_queue_length += queue_length;

            if (customer_index < max_customers) {
                next_arrival = current_time + arrival_dist(mt);
            }
            else {
                next_arrival = INF;
            }
            customer_index++;

            if (!is_server_busy) {
                is_server_busy = true;
                next_departure = current_time + service_dist(mt);
            }
            else {
                queue_length++;
            }
        }
        else {
            cout << "; Выход (Уход)\n";

            if (queue_length == 0) {
                is_server_busy = false;
                next_departure = INF;
            }
            else {
                queue_length--;
                next_departure = current_time + service_dist(mt);
            }
        }
    }

    cout << "Средняя длина очереди: " << total_queue_length / max_customers << "\n";

    return 0;
}
#endif


// =====================================================================
// 8. Comp_lab_4.cpp
// =====================================================================
#ifdef RUN_COMP_LAB_4_SUPERPOSITION
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <chrono>
#include <iomanip>
#include <cmath>
#include <clocale>

using namespace std;
using namespace chrono;

struct Event {
    double time;
    int stream_id;

    bool operator<(const Event& other) const {
        return time < other.time;
    }
};


vector<Event> generateStream(int id, double a, double b, double T, mt19937& gen) {
    vector<Event> stream;
    uniform_real_distribution<double> interval_dist(a, b);

    double t = interval_dist(gen);
    while (t < T) {
        stream.push_back({ t, id });
        t += interval_dist(gen);
    }
    return stream;
}

struct Stats {
    double mean;   
    double var;    
    double cv;     
    double lambda; 
};

Stats computeStats(const vector<double>& intervals) {
    double n = (double)intervals.size();
    double sum = 0.0;
    for (double x : intervals) sum += x;
    double mean = sum / n;

    double sq = 0.0;
    for (double x : intervals) sq += (x - mean) * (x - mean);
    double var = sq / n;
    double sd = sqrt(var);

    Stats s;
    s.mean = mean;
    s.var = var;
    s.cv = sd / mean;
    s.lambda = 1.0 / mean;
    return s;
}

int main() {
    setlocale(LC_ALL, "");
    cout << fixed << setprecision(4);

    double a, b;
	cout << "Введите параметры интервала a и b (a < b): ";
    cin >> a;
    cin >> b;
    double T = 200000.0;

    mt19937 gen{ static_cast<mt19937::result_type>(
        steady_clock::now().time_since_epoch().count()) };

    double lambda1_theory = 2.0 / (a + b);

    vector<int> Ns = { 1, 2, 5, 10, 20, 50, 100 };

    cout << "Параметры отдельного потока: интервалы ~ U(" << a << ", " << b << ")\n";
    cout << "Теоретическая интенсивность одного потока: lambda1 = " << lambda1_theory << "\n\n";

    for (int N : Ns) {
        vector<Event> events;

        for (int i = 0; i < N; ++i) {
            vector<Event> stream = generateStream(i, a, b, T, gen);
            events.insert(events.end(), stream.begin(), stream.end());
        }

        sort(events.begin(), events.end());

        vector<double> intervals;
        for (size_t i = 1; i < events.size(); ++i)
            intervals.push_back(events[i].time - events[i - 1].time);

        Stats st = computeStats(intervals);
        double lambda_theory_N = N * lambda1_theory;

        cout << "N = " << N << " потоков\n";
        cout << "Число событий суммарного потока: " << events.size() << "\n";
        cout << "Среднее значение интервала (эмпир.):        " << st.mean << "\n";
        cout << "Интенсивность (эмпир., lambda=1/M[tau]):    " << st.lambda << "\n";
        cout << "Интенсивность (теор., N*lambda1):           " << lambda_theory_N << "\n";
        cout << "Коэфф. вариации интервалов CV = sigma/M:    " << st.cv
            << "  (для пуассоновского потока CV = 1)\n";

        int bins = 15;
        double maxT = 0;
        for (double x : intervals) maxT = max(maxT, x);
        vector<int> hist(bins, 0);
        double binWidth = maxT / bins;
        for (double x : intervals) {
            int idx = min(bins - 1, (int)(x / binWidth));
            hist[idx]++;
        }

        cout << "Плотность интервалов: эмпирическая против теоретической экспоненциальной f(x)=lambda*e^(-lambda*x)\n";
        cout << left << setw(18) << "Интервал"
            << setw(16) << "Эмпир. f(x)"
            << "Теор. f(x)\n";
        for (int i = 0; i < bins; ++i) {
            double x_left = i * binWidth;
            double x_right = (i + 1) * binWidth;
            double x_mid = (x_left + x_right) / 2.0;
            double f_emp = hist[i] / (double)intervals.size() / binWidth;
            double f_theory = st.lambda * exp(-st.lambda * x_mid);
            cout << "[" << setw(6) << x_left << "," << setw(6) << x_right << "] "
                << setw(16) << f_emp << f_theory << "\n";
        }
        cout << "\n";
    }

    return 0;
}
#endif


// =====================================================================
// 9. Comp_model_lab_5.cpp
// =====================================================================
#ifdef RUN_COMP_LAB_5_MULTIPHASE
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <queue>
#include <algorithm>

using namespace std;
using namespace chrono;

struct Job {
    double arrival_time;
    int k; 
    int m; 
};

int main() {
    setlocale(LC_ALL, "ru");

    mt19937 mt{ static_cast<mt19937::result_type>(
        steady_clock::now().time_since_epoch().count()
    ) };

    double a, b;
    cout << "Введите a и b (a < b): ";
    cin >> a >> b;

    int k_max, m_max;
    cout << "Введите k (по условию < 4): ";
    cin >> k_max;
    cout << "Введите m (по условию < 3): ";
    cin >> m_max;

    double E_k = (1.0 + k_max) / 2.0;
    double E_m = (1.0 + m_max) / 2.0;
    double min_ab = max(2.0 * E_k * 11.0, 2.0 * E_m * 17.0);

    if (a + b <= min_ab) {
        cout << "\n a + b = " << a + b << " <= " << min_ab << ".\n";
        cout << "Система перегружена. очередь будут бесконечно расти\n\n";
    }
    else {
        cout << "\n[+] Система устойчива (a + b = " << a + b << " > " << min_ab << "\n\n";
    }

    uniform_real_distribution<double> tau{ a, b };
    uniform_int_distribution<int> dist_k{ 1, k_max };
    uniform_int_distribution<int> dist_m{ 1, m_max };

    int K1 = 0, K2 = 0;

    double T1, T2_1 = 1e9, T2_2 = 1e9, systemtime = 0.0;
    int TYP = 1, i = 1, N = 100000;
    int total_completed = 0;

    double sigma1 = 11.0;
    double sigma2 = 17.0;

    queue<Job> q1;
    queue<Job> q2;

    Job current_job1, current_job2; 
    double total_time_in_system = 0.0;

    T1 = tau(mt);

    while (total_completed < N) {
        systemtime = min({ T1, T2_1, T2_2 });

        if (systemtime == T1) TYP = 1;
        else if (systemtime == T2_1) TYP = 2;
        else if (systemtime == T2_2) TYP = 3;

        if (TYP == 1) {
            if (i < N) {
                T1 = systemtime + tau(mt);
            }
            else {
                T1 = 1e9;
            }
            i++;

            Job newJob = { systemtime, dist_k(mt), dist_m(mt) };

            if (K1 == 0) {
                K1 = 1;
                current_job1 = newJob;
                T2_1 = systemtime + sigma1; 
            }
            else {
                q1.push(newJob);
            }
        }
        else if (TYP == 2) {
            current_job1.k--;

            if (current_job1.k > 0) {
                q1.push(current_job1);
            }
            else {
                if (K2 == 0) {
                    K2 = 1;
                    current_job2 = current_job1;
                    T2_2 = systemtime + sigma2;
                }
                else {
                    q2.push(current_job1);
                }
            }

            if (!q1.empty()) {
                current_job1 = q1.front();
                q1.pop();
                T2_1 = systemtime + sigma1;
            }
            else {
                K1 = 0;
                T2_1 = 1e9;
            }
        }
        else if (TYP == 3) {
            current_job2.m--;

            if (current_job2.m > 0) {
                q2.push(current_job2);
            }
            else {
                total_time_in_system += (systemtime - current_job2.arrival_time);
                total_completed++;
            }

            if (!q2.empty()) {
                current_job2 = q2.front();
                q2.pop();
                T2_2 = systemtime + sigma2;
            }
            else {
                K2 = 0;
                T2_2 = 1e9;
            }
        }
    }

    cout << "Всего обработано заданий: " << total_completed << "\n";
    cout << "Среднее время пребывания в системе: " << total_time_in_system / N << "\n";

    return 0;
}
#endif


// =====================================================================
// 10. Безымянный код 1 (интервалы, векторы streams)
// =====================================================================
#ifdef RUN_UNNAMED_1_INTERVALS
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    mt19937 mt{ static_cast<mt19937::result_type>(
        chrono::steady_clock::now().time_since_epoch().count()
    ) };

    int n = 10;
    double a, b;
    cout << "\nВведите a и b: ";
    cin >> a >> b;
    double T = 1000.0; 
    int task = 100;
    uniform_real_distribution<double> dist{ a, b };

    vector<vector<double>> streams(n);
    
    for (int i = 0; i < n; i++)
    {
        long double time = 0.0;
            while (time < T)
            {
                double interval = dist(mt);
                time += interval;
                if (time <= T)
                {
                    streams[i].push_back(time);
                }
            }
    }

    double total_task = 0;
    for (int i = 0; i < n; i++)
    {
        double lambda_i = streams[i].size() / T;
        cout << "Поток " << i + 1 << ": " << streams[i].size() << " интенсивность = " << lambda_i << "\n";
        total_task += streams[i].size();
    }

    long double real_lambda = n / ((a + b) / 2.0);
    cout << "lambda_teor = " << n << "/" << (a + b) / 2.0 << " = " << real_lambda << "\n";

    vector<double> superposition;
    for (int i = 0; i < n; i++)
    {
        for (double t : streams[i]) {
            superposition.push_back(t);
        }
    }
    sort(superposition.begin(), superposition.end());

    vector<double> interval;
    for (size_t i = 1; i < superposition.size(); i++) {
        interval.push_back(superposition[i] - superposition[i - 1]);
    }

    double sum_int = 0;
    for (double inter : interval) {
        sum_int += inter;
    }

    double avg_inter = sum_int / interval.size();
    long double emp_lambda = 1.0 / avg_inter;

    cout << "\nВсего событий: " << superposition.size() << "\n";
    cout << "Длительность: " << T << "\n";
    cout << "Эмперическая интенсивность: " << emp_lambda << "\n";
    cout << "Теоритическая интенсивность: " << real_lambda << "\n";
    cout << "Отклонение: " << abs(emp_lambda - real_lambda) << "\n";
    int max_int = 10;

    vector<int> counts(max_int + 1, 0);

    for (double interv : interval)
    {
        int idx = (int)interv;
        if (idx > max_int)
            idx = max_int;
        counts[idx]++;
    }

    cout << "Интервал\tЧисло\tВероятность\n";
    for (int i = 0; i < max_int; i++) {
        double prob = (double)counts[i] / interval.size();
        if (i < max_int) {
            cout << i << "-" << i + 1 << "\t\t" << counts[i] << "\t\t" << setprecision(4) << prob << "\n";
        }
        else
            cout << i << "+\t\t" << counts[i] << "\t\t" << setprecision(4) << prob << "\n";
    }
    return 0;
}
#endif


// =====================================================================
// 11. Безымянный код 2 (простая очередь tau/sigma)
// =====================================================================
#ifdef RUN_UNNAMED_2_QUEUE_SIMPLE
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <algorithm>

using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    mt19937 mt{ static_cast<mt19937::result_type>(
        chrono::steady_clock::now().time_since_epoch().count()
    ) };

    uniform_real_distribution<double> tau{ 1.0, 7.0 };
    uniform_real_distribution<double> sigma{ 1.0, 6.0 };

    int K = 0, TYP = 1, i = 1, L = 0, N = 10;
    double systemtime = 0.0, T1, T2, ML = 0.0;

    T1 = tau(mt);
    T2 = T1 + sigma(mt);

    while (i <= N) {
        systemtime = min(T1, T2);

        if (T1 <= T2) {
            TYP = 1;
        }
        else {
            TYP = 2;
        }

        cout << "state (K = " << K << " L = " << L << ") t = " << systemtime;

        if (TYP == 1) {
            cout << "; In i = " << i << " TYP = " << TYP << "\n";
            ML += L;

            if (i < N) {
                T1 = systemtime + tau(mt);
            }
            else {
                T1 = 1e9;
            }
            i++;

            if (K == 0) {
                K = 1;
                T2 = systemtime + sigma(mt);
            }
            else {
                L++;
            }
        }
        else {
            cout << "; Out: TYP = " << TYP << "\n";

            if (L == 0) {
                K = 0;
                T2 = 1e9;
            }
            else {
                L--;
                T2 = systemtime + sigma(mt);
            }
        }
    }

    cout << "Средняя длина очереди: " << ML / N << "\n";

    return 0;
}
#endif


// =====================================================================
// 12. Безымянный код 3 (очередь M/M/1, F(x), ожидание)
// =====================================================================
#ifdef RUN_UNNAMED_3_QUEUE_WAIT
#include <iostream>
#include <random>
#include <chrono>
#include <iomanip>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int main() {
    setlocale(LC_ALL, "ru");

    mt19937 mt{ static_cast<mt19937::result_type>(
        chrono::steady_clock::now().time_since_epoch().count()
    ) };

    int K = 0, TYP = 1, i = 1, L = 0, N = 100000;
    int total = 0; 
    long double systemtime = 0.0, T1, T2 = 1e9, ML = 0.0;

    double lambda = 0.24;
    exponential_distribution<double> tau{ lambda };
    double mu = 0.3;
    exponential_distribution<double> sigma{ mu };

    vector<long double> arrival;
    vector<long double> wait;

    T1 = tau(mt);

    while (total < N) {
        systemtime = min(T1, T2);

        if (T1 <= T2) {
            TYP = 1;
        }
        else {
            TYP = 2;
        }

        if (TYP == 1) {
            ML += L;

            if (i < N) {
                T1 = systemtime + tau(mt);
            }
            else {
                T1 = 1e9;
            }
            i++;

            if (K == 0) {
                K = 1;
                T2 = systemtime + sigma(mt);
                wait.push_back(0.0);
                total++;
            }
            else {
                L++;
                arrival.push_back(systemtime);
            }
        }
        else {
            if (L == 0) {
                K = 0;
                T2 = 1e9;
            }
            else {
                L--;

                uniform_int_distribution<size_t> dist(0, arrival.size() - 1);
                size_t random_idx = dist(mt);

                double arrival_time = arrival[random_idx];
                arrival.erase(arrival.begin() + random_idx);

                double wait_doub = systemtime - arrival_time;
                wait.push_back(wait_doub);
                total++;

                T2 = systemtime + sigma(mt);
            }
        }
    }

    cout << "Распределение и функция распределения:\n";
    cout << "Интервал\tЧисло задач\tВероятность\tF(x)\n";

    int all_interval = 15;
    vector<int> counts(all_interval + 1, 0);

    for (int i = 0; i < wait.size(); i++) {
        int idx = round(wait[i]);
        if (idx > all_interval) {
            idx = all_interval;
        }
        counts[idx]++;
    }

    double res_func = 0.0;
    for (int j = 0; j <= all_interval; j++) {
        double chast = 1.0 * counts[j] / N; 
        res_func += chast;
        if (j < all_interval) {
            cout << j << "-" << j + 1 << " интервал\t" << counts[j] << "\t\t" << setprecision(4) << chast << "\t\t" << setprecision(4) << res_func << "\n";
        }
        else {
            cout << j << " п. интервал\t" << counts[j] << "\t\t" << setprecision(4) << chast << "\t\t" << setprecision(4) << res_func << "\n";
        }
    }

    cout << "Средняя длина очереди: " << ML / N << "\n";
    cout << "Среднее время ожидания: ";
    double sum_wait = 0.0;
    for (int i = 0; i < wait.size(); i++) {
        sum_wait += wait[i];
    }
    cout << sum_wait / N << "\n";

    return 0;
}
#endif