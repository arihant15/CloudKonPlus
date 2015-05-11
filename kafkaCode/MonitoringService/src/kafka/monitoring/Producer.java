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
package kafka.monitoring;


//import java.util.Properties;
//import java.util.concurrent.ExecutionException;
import java.util.*;

import java.io.*;

import org.apache.kafka.clients.producer.KafkaProducer;
import org.apache.kafka.clients.producer.ProducerConfig;
import org.apache.kafka.clients.producer.ProducerRecord;
import org.apache.kafka.common.serialization.ByteArraySerializer;
import org.apache.kafka.common.serialization.StringSerializer;

import org.apache.avro.io.BinaryDecoder;
import org.apache.avro.io.BinaryEncoder;
import org.apache.avro.io.DecoderFactory;
import org.apache.avro.io.EncoderFactory;
import org.apache.avro.specific.SpecificDatumReader;
import org.apache.avro.specific.SpecificDatumWriter;
import org.apache.avro.util.ByteBufferInputStream;
//import org.apache.commons.io.IOUtils;

public class Producer extends Thread implements KafkaProperties
{
  private final KafkaProducer<Integer, byte[]> producer;
  private final String topic;
  private final Properties props = new Properties();
  
  private static final SpecificDatumWriter<Worker> avroEventWriter = new SpecificDatumWriter<Worker>(Worker.SCHEMA$);
  private static final EncoderFactory avroEncoderFactory = EncoderFactory.get();

  private Worker worker;  

  private static Producer producerInstance = new Producer();
  private Producer() 
  {
    props.put(ProducerConfig.BOOTSTRAP_SERVERS_CONFIG, KafkaProperties.kafkaServerURL + ":" + KafkaProperties.kafkaServerPort);
    props.put(ProducerConfig.VALUE_SERIALIZER_CLASS_CONFIG,  "org.apache.kafka.common.serialization.ByteArraySerializer");
    props.put(ProducerConfig.KEY_SERIALIZER_CLASS_CONFIG, "org.apache.kafka.common.serialization.StringSerializer");
    // Use random partitioner. Don't need the key type. Just set it to Integer.
    // The message is of type String.
    producer = new KafkaProducer<Integer, byte[]>(props);
    this.topic = KafkaProperties.topic;
    worker = new Worker();
  }

  public static Producer getInstance() {
    return producerInstance;
  }
  
  public void setSchema(String workerName, String workerId, String state, long timestamp, List<CharSequence> tags) {
      worker.setWorkerName(workerName);
      worker.setWorkerId(workerId);
      worker.setState(state);
      worker.setTimestamp(timestamp);
      worker.setTags(tags);
  }

  @Override
  public void run() {
    publish();
  }
  
  public void publish() {     
      try {
        ByteArrayOutputStream stream = new ByteArrayOutputStream();
        BinaryEncoder binaryEncoder = avroEncoderFactory.binaryEncoder(stream, null);
        avroEventWriter.write(worker, binaryEncoder);
        binaryEncoder.flush();
        stream.close();
        //IOUtils.closeQuietly(stream);

        producer.send(new ProducerRecord<Integer, byte[]>(topic, stream.toByteArray()));
      }
      catch(Exception e) {
    	  e.printStackTrace();
        throw new RuntimeException("Avro serilization failure", e);
      }
  }

  public static void main(String args[]) {
	  
  }
}
