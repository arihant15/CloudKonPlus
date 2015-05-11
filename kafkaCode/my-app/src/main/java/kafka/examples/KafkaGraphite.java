package kafka.examples;

import java.io.File;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.Calendar;
import java.util.LinkedList;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

import org.rrd4j.core.Util;

public class KafkaGraphite implements Runnable {
	Worker worker = null;
	String taskType = null;
	Path currentRelativePath = Paths.get("");
	String absPath = currentRelativePath.toAbsolutePath().toString();
	
	public static Map<String, LinkedList<Long>> workerStore = new ConcurrentHashMap<String, LinkedList<Long>>();

	static final int IMG_WIDTH = 500;
	static final int IMG_HEIGHT = 300;
	String METRICNAME;

	public KafkaGraphite(Worker worker, String taskType) {
		this.worker = worker;
		METRICNAME = worker.getWorkerId() + "_" + taskType;
		this.taskType = taskType;
	}

	public KafkaGraphite() {

	}

	public void run() {
		try {
			if (taskType.equalsIgnoreCase(MetricType.AVG_TASK)) {
				tasksAvgTaskMetric();
			} else if (taskType.equalsIgnoreCase(MetricType.STATE_TASK)) {
				tasksStateMetric();
			}
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	private void tasksAvgTaskMetric() {
		long old = 0;
		if (Consumer.valueStore.get(METRICNAME) == null) {
			Consumer.valueStore.put(METRICNAME, (Object) 0L);
		} else {
			old = (Long) Consumer.valueStore.get(METRICNAME);
		}

		if (workerStore.get(METRICNAME) == null) {
			workerStore.put(METRICNAME, new LinkedList<Long>());
			new Thread(new WorkerThread(METRICNAME)).start();
		} else {
			workerStore.get(METRICNAME).addLast(worker.getTimestamp() - old);
		}
		Consumer.valueStore.put(METRICNAME, (Object) worker.getTimestamp());
	}

	private void tasksStateMetric() {
		String COMMA_DELIMITER = ",";
		String NEW_LINE_SEPARATOR = "\n";
		String FILE_HEADER = "state,time,sec";
		String csvPath = absPath + METRICNAME + ".csv";

		FileWriter fileWriter = null;

		try {
			File file = new File(csvPath);
			fileWriter = new FileWriter(csvPath, true);
			if (!file.exists()) {
				fileWriter.append(FILE_HEADER.toString());
				fileWriter.append(NEW_LINE_SEPARATOR);
			}

			fileWriter.append(worker.getState().toString());
			fileWriter.append(COMMA_DELIMITER);
			java.util.Calendar calendar = Util.getCalendar(worker
					.getTimestamp());
			fileWriter.append(String.valueOf(calendar.get(Calendar.HOUR) +
					":"+ String.valueOf(calendar.get(Calendar.MINUTE)
					+ ":" + calendar.get(Calendar.SECOND))));
			fileWriter.append(COMMA_DELIMITER);
			fileWriter.append(String.valueOf(calendar.getTimeInMillis() * 0.001));
			fileWriter.append(NEW_LINE_SEPARATOR);
			System.out.println("printed");
		} catch (IOException e) {
			e.printStackTrace();
		} finally {

			try {
				fileWriter.flush();
				fileWriter.close();
			} catch (IOException e) {
				System.out
						.println("Error while flushing/closing fileWriter !!!");
				e.printStackTrace();
			}

		}

	}

	private class WorkerThread implements Runnable {
		// Worker w = null;
		String WORKERNAME;

		String COMMA_DELIMITER = ",";
		String NEW_LINE_SEPARATOR = "\n";
		String FILE_HEADER = "avgtime,date,secs";
		String csvPath;

		public WorkerThread(String m) {
			// this.w = w;
			WORKERNAME = m;
			csvPath = absPath + m + ".csv";
		}

		public void run() {
			// TODO Auto-generated method stub

			FileWriter fileWriter = null;
			for (;;) {

				try {
					Thread.sleep(60000);

					File file = new File(csvPath);
					fileWriter = new FileWriter(csvPath, true);
					System.out.println("printed");		if (!file.exists()) {
						fileWriter.append(FILE_HEADER.toString());
						fileWriter.append(NEW_LINE_SEPARATOR);
					}

					LinkedList<Long> times = KafkaGraphite.workerStore
							.get(WORKERNAME);
					if (times.size() <= 0) {
						continue;
					}
					System.out.println("printed avg");
					int size = times.size();
					
					long sum = 0L;

					for (int i = 0; i < size; i++) {
						sum += times.pollFirst();
					}
					
					System.out.println(sum);
					fileWriter.append(String.valueOf((sum * 0.001) / 60));
					fileWriter.append(COMMA_DELIMITER);
					java.util.Calendar calendar = Util.getCalendar(System.currentTimeMillis());
					fileWriter.append(String.valueOf(calendar
							.get(Calendar.HOUR) + ":" + String.valueOf(calendar
							.get(Calendar.MINUTE)
							+ ":"
							+ calendar.get(Calendar.SECOND))));
					fileWriter.append(COMMA_DELIMITER);
					fileWriter.append(String.valueOf(calendar
							.getTimeInMillis() * 0.001));
					fileWriter.append(NEW_LINE_SEPARATOR);

				} catch (IOException e) {
					e.printStackTrace();
				} catch (InterruptedException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				} catch (Exception e) {
					// TODO: handle exception
					e.printStackTrace();
				} finally {

					try {
						fileWriter.flush();
						fileWriter.close();
					} catch (IOException e) {
						System.out
								.println("Error while flushing/closing fileWriter !!!");
						e.printStackTrace();
					} catch (NullPointerException e2) {
						// TODO: handle exception
						e2.printStackTrace();
					}

				}
			}
		}
	}

	// public static void main(String args[]) {
	// KafkaGraphite k = new KafkaGraphite();
	// k.tasksReceivedMetric();
	// }
}
