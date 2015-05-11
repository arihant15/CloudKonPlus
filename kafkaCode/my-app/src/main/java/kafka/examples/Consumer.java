/**
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 * 
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package kafka.examples;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.Executors;
import java.util.concurrent.ExecutorService;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Properties;

import org.apache.avro.io.Decoder;
import org.apache.avro.io.DecoderFactory;
import org.apache.avro.specific.SpecificDatumReader;
import org.apache.log4j.Logger;

import kafka.consumer.ConsumerConfig;
import kafka.consumer.ConsumerIterator;
import kafka.consumer.KafkaStream;
import kafka.javaapi.consumer.ConsumerConnector;

public class Consumer extends Thread {
	static Logger logger = Logger.getLogger(Consumer.class);
	private final ConsumerConnector consumer;
	private final String topic;
	private String taskType;
	
	public static final long TIMESTAMP = System.currentTimeMillis();
	public static long LASTTIMESTAMP = System.currentTimeMillis();
	public static Map<String, Object> valueStore = new ConcurrentHashMap<String, Object>();
	
	ExecutorService threadPool;

	public Consumer(String topic, String taskType) {
		threadPool = Executors.newCachedThreadPool();
		consumer = kafka.consumer.Consumer
				.createJavaConsumerConnector(createConsumerConfig());
		this.topic = topic;
		this.taskType = taskType;
	}

	private static ConsumerConfig createConsumerConfig() {
		Properties props = new Properties();
		props.put("zookeeper.connect", KafkaProperties.zkConnect);
		props.put("group.id", KafkaProperties.groupId);
		props.put("zookeeper.session.timeout.ms", "10000");
		props.put("zookeeper.sync.time.ms", "200");
		props.put("auto.commit.interval.ms", "1000");

		return new ConsumerConfig(props);

	}

	public void run() {
		System.out.println("Entered the consumer thread");
		Map<String, Integer> topicCountMap = new HashMap<String, Integer>();
		topicCountMap.put(topic, new Integer(1));
		System.out.println("Entered the consumer thread1");
		Map<String, List<KafkaStream<byte[], byte[]>>> consumerMap = consumer
				.createMessageStreams(topicCountMap);
		KafkaStream<byte[], byte[]> stream = consumerMap.get(topic).get(0);
		System.out.println("Entered the consumer thread2");
		ConsumerIterator<byte[], byte[]> it = stream.iterator();
		System.out.println("Entered the consumer thread3");
		while (it.hasNext()) {
			try {
				SpecificDatumReader<Worker> reader = new SpecificDatumReader<Worker>(
						Worker.getClassSchema());
				byte[] b = it.next().message();
				Decoder decoder = DecoderFactory.get().binaryDecoder(b, null);
				Worker w = reader.read(null, decoder);
				
				if (taskType.equalsIgnoreCase(MetricType.AVG_TASK)) {
					if (w.getState().toString().equalsIgnoreCase("1")) {
						threadPool.submit(new KafkaGraphite(w, taskType));
					}
				}
				
				if (taskType.equalsIgnoreCase(MetricType.STATE_TASK)) {
					threadPool.submit(new KafkaGraphite(w, taskType));
				}
			} catch (Exception e) {
				e.printStackTrace();
				System.out.println(e);
			}
		}
		// System.out.println("Entered the consumer thread4");
	}
}
