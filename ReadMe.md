Hello and welcome to the GitHub of the Dynamic Double anomaly Detection (DyD²) algorithm.
Paper available here: https://hal.laas.fr/hal-03609573v2


-------------------------------------------------------------------------------------------------------------------

The code here provide all the inner functions of DyD². Moreover, some data sets are given as exampke to experiment.
These data sets come from heavy-ion testing kindly provided by the CNES. It uses the TILU2 equipment to monitor the events:
https://escies.org/download/webDocumentFile?id=63064

For now only these data sets are available but other coming from computer simulations will be added.


To test the algorithm, refer to the paramters.h file where it is possible to interact with DyD². 
Note that a build is needed to take the modifications into consideration. 
A makefile is available to run with custom parameters. 
For Linuw users: simply run *make* in the terminal.
For Windows users, you have to download a version of GCC (see: https://dev.to/gamegods3/how-to-install-gcc-in-windows-10-the-easier-way-422j).
Then, run *mingw32-make* in the terminal.

When launching vanilla DyD².exe, it is doing a run on the data sets, giving the confusion matrix.


-------------------------------------------------------------------------------------------------------------------


Here is a quick introduction to DyD²:
At the core of DyD² is a space application that is going to be referred to throughout the description.

Space applications impose many constraints that machine learning algorithms generally do not meet. 
Therefore, the first step of this work is to define the requirements to be fulfilled by a machine learning algorithm to detect single event effects in the case of an on-board space mission. 
Thus, the machine learning algorithm DYD2 has been designed to detect single effects in space missions while satisfying the requirements below:

	1)	Change point anomaly detection in time series: The algorithm must be able to detect anomalies in data sets that take the form of time series. Those anomalies are defined as anomalous change points in the time series. In a space application, anomalies can be considered as a high current event in the supply current. Moreover, the detection algorithm should be able to detect critical and destructive SEE, such as single event latch-up, accurately. Finally, it must be able to detect soft-errors that can pass through the baseline antilatch-up system. DYD2 is specifically designed to handle time series. Furthermore, a change point detection followed by a two-phase anomaly detection algorithm enables DYD2 to detect anomalous behaviours.

	2)	Low memory usage: The algorithm must be able to be embedded and run on minimal resources. Indeed, some space missions are designed for microcontrollers with only a few Kbytes of flash memory. Therefore, the available memory space must be optimised as much as possible. DYD2 is based on specific objects called µ-clusters that group together similar samples. Therefore, only the µ-clusters need to be stored in memory, as opposed to the entirety of the data set, drastically decreasing memory usage.

	3)	Real-time detection: The algorithm must be able to run efficiently in real-time in regards to the capability of space components. With the reliability of the monitored component at stakes, the algorithm must be able to compute new samples as they arrive without delay. DYD2 uses a fast change point detection to classify new samples as potential anomalies. Doing so avoids processing non-anomalous data points, thus saving time. Also, the µ-clusters design accelerates the detection process, as fewer objects need to be processed, as opposed to the entire data set.

	4)	Adaptability in evolving environment: The algorithm must be able to adapt to a constantly changing environment. Indeed, due to the total ionising dose, a component behaviour evolves during the entirety of the mission. These evolutions modify significantly the supply current, so any training performed beforehand becomes irrelevant. Moreover, it can be complex to model the ageing process, as many factors influence it (e.g type of component, process, internal structures). By the use of an update phase that is constantly adapting to incoming data, DYD2 is able to follow the deviation of a data set and does not require additional training for that.

	5)	Training on normal behaviour only: The training phase must be performed using only normal data. Indeed, the simulation of single event effects can be tedious and complex, and removing the need for extensive radiation testing is crucial. For example, during our previous study , two main scientific hurdles emerged. First, it is challenging to characterise correctly the type of anomaly appearing during testing (such as SEL, SEGR, SEFI, SEU). Moreover, it is complicated to get an extensive database of all possible anomalies for complex components, such as a microcontroller. Therefore, the database can only be partially created, and so, the quality of the prediction of a machine learning algorithm can be severely altered. DYD2 is part of a sub-field of machine learning called one-class classification in which only one type of data is needed to create a model: data of the normal behaviour.

	6)	Interpretability: Finally, the last requirement to consider this work is linked to the nature of artificial intelligence fields, and the apparition of black-box models. The algorithm must be as much interpretable, or explainable, as possible. It is well-known that it is complicated to determine precisely how a deep learning algorithm gives a specific prediction. The possibility to explain and understand the results of a model is called interpretability. In the case of space application, the possibility to interpret the prediction seems essential to apply with confidence machine learning detection for radiation faults. It is why tools such as neural networks are not investigated in this work. There is no clear definition to evaluate the interpretability of a model yet, but some indicators can improve its comprehension. DYD2 is a deterministic algorithm. Its predictions are not based on probabilities and does not need random initialisation. Also, the evaluation of an anomaly is based on easily explicable concepts such as the notion of reachability and µ-clusters. Finally, tools are developed to visualised the evolution of DYD2 through a data set, giving the possibility to explain its predictions.




