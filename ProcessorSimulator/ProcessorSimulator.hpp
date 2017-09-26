#pragma once
#include "Shared.hpp"
namespace cs {
	class TaskGenerator;
	class TaskStorage;
	class TaskProcessor;

	enum class StorageType {
		LIFO, PER
	};

	class ProcessorSimulator {
		number m_time_coefficient;
		TaskGenerator *m_generator;
		TaskStorage *m_storage;
		TaskProcessor *m_processor;
	public:
		void initialize(StorageType type);
		void start();
		void pause();
		void stop();

		void changeTimeCoefficient(number c);
		void changeLambda(number l);
		void changeMu(number m);
		void changeSigma(number s);
		void changeTau(number t);

		inline TaskGenerator* generator() { return m_generator; }
		inline TaskStorage* storage() { return m_storage; }
		inline TaskProcessor* processor() { return m_processor; }

		bool is_running();
		StorageType type();
	};
}