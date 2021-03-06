/**
 * Autogenerated by Avro
 * 
 * DO NOT EDIT DIRECTLY
 */
package kafka.monitoring;  
@SuppressWarnings("all")
@org.apache.avro.specific.AvroGenerated
public class Worker extends org.apache.avro.specific.SpecificRecordBase implements org.apache.avro.specific.SpecificRecord {
  public static final org.apache.avro.Schema SCHEMA$ = new org.apache.avro.Schema.Parser().parse("{\"type\":\"record\",\"name\":\"Worker\",\"namespace\":\"kafka.monitoring\",\"fields\":[{\"name\":\"worker_name\",\"type\":\"string\"},{\"name\":\"worker_id\",\"type\":\"string\"},{\"name\":\"tags\",\"type\":[\"null\",{\"type\":\"array\",\"items\":\"string\"}]},{\"name\":\"state\",\"type\":\"string\"},{\"name\":\"timestamp\",\"type\":\"long\"}]}");
  public static org.apache.avro.Schema getClassSchema() { return SCHEMA$; }
  @Deprecated public java.lang.CharSequence worker_name;
  @Deprecated public java.lang.CharSequence worker_id;
  @Deprecated public java.util.List<java.lang.CharSequence> tags;
  @Deprecated public java.lang.CharSequence state;
  @Deprecated public long timestamp;

  /**
   * Default constructor.  Note that this does not initialize fields
   * to their default values from the schema.  If that is desired then
   * one should use <code>newBuilder()</code>. 
   */
  public Worker() {}

  /**
   * All-args constructor.
   */
  public Worker(java.lang.CharSequence worker_name, java.lang.CharSequence worker_id, java.util.List<java.lang.CharSequence> tags, java.lang.CharSequence state, java.lang.Long timestamp) {
    this.worker_name = worker_name;
    this.worker_id = worker_id;
    this.tags = tags;
    this.state = state;
    this.timestamp = timestamp;
  }

  public org.apache.avro.Schema getSchema() { return SCHEMA$; }
  // Used by DatumWriter.  Applications should not call. 
  public java.lang.Object get(int field$) {
    switch (field$) {
    case 0: return worker_name;
    case 1: return worker_id;
    case 2: return tags;
    case 3: return state;
    case 4: return timestamp;
    default: throw new org.apache.avro.AvroRuntimeException("Bad index");
    }
  }
  // Used by DatumReader.  Applications should not call. 
  @SuppressWarnings(value="unchecked")
  public void put(int field$, java.lang.Object value$) {
    switch (field$) {
    case 0: worker_name = (java.lang.CharSequence)value$; break;
    case 1: worker_id = (java.lang.CharSequence)value$; break;
    case 2: tags = (java.util.List<java.lang.CharSequence>)value$; break;
    case 3: state = (java.lang.CharSequence)value$; break;
    case 4: timestamp = (java.lang.Long)value$; break;
    default: throw new org.apache.avro.AvroRuntimeException("Bad index");
    }
  }

  /**
   * Gets the value of the 'worker_name' field.
   */
  public java.lang.CharSequence getWorkerName() {
    return worker_name;
  }

  /**
   * Sets the value of the 'worker_name' field.
   * @param value the value to set.
   */
  public void setWorkerName(java.lang.CharSequence value) {
    this.worker_name = value;
  }

  /**
   * Gets the value of the 'worker_id' field.
   */
  public java.lang.CharSequence getWorkerId() {
    return worker_id;
  }

  /**
   * Sets the value of the 'worker_id' field.
   * @param value the value to set.
   */
  public void setWorkerId(java.lang.CharSequence value) {
    this.worker_id = value;
  }

  /**
   * Gets the value of the 'tags' field.
   */
  public java.util.List<java.lang.CharSequence> getTags() {
    return tags;
  }

  /**
   * Sets the value of the 'tags' field.
   * @param value the value to set.
   */
  public void setTags(java.util.List<java.lang.CharSequence> value) {
    this.tags = value;
  }

  /**
   * Gets the value of the 'state' field.
   */
  public java.lang.CharSequence getState() {
    return state;
  }

  /**
   * Sets the value of the 'state' field.
   * @param value the value to set.
   */
  public void setState(java.lang.CharSequence value) {
    this.state = value;
  }

  /**
   * Gets the value of the 'timestamp' field.
   */
  public java.lang.Long getTimestamp() {
    return timestamp;
  }

  /**
   * Sets the value of the 'timestamp' field.
   * @param value the value to set.
   */
  public void setTimestamp(java.lang.Long value) {
    this.timestamp = value;
  }

  /** Creates a new Worker RecordBuilder */
  public static kafka.monitoring.Worker.Builder newBuilder() {
    return new kafka.monitoring.Worker.Builder();
  }
  
  /** Creates a new Worker RecordBuilder by copying an existing Builder */
  public static kafka.monitoring.Worker.Builder newBuilder(kafka.monitoring.Worker.Builder other) {
    return new kafka.monitoring.Worker.Builder(other);
  }
  
  /** Creates a new Worker RecordBuilder by copying an existing Worker instance */
  public static kafka.monitoring.Worker.Builder newBuilder(kafka.monitoring.Worker other) {
    return new kafka.monitoring.Worker.Builder(other);
  }
  
  /**
   * RecordBuilder for Worker instances.
   */
  public static class Builder extends org.apache.avro.specific.SpecificRecordBuilderBase<Worker>
    implements org.apache.avro.data.RecordBuilder<Worker> {

    private java.lang.CharSequence worker_name;
    private java.lang.CharSequence worker_id;
    private java.util.List<java.lang.CharSequence> tags;
    private java.lang.CharSequence state;
    private long timestamp;

    /** Creates a new Builder */
    private Builder() {
      super(kafka.monitoring.Worker.SCHEMA$);
    }
    
    /** Creates a Builder by copying an existing Builder */
    private Builder(kafka.monitoring.Worker.Builder other) {
      super(other);
      if (isValidValue(fields()[0], other.worker_name)) {
        this.worker_name = data().deepCopy(fields()[0].schema(), other.worker_name);
        fieldSetFlags()[0] = true;
      }
      if (isValidValue(fields()[1], other.worker_id)) {
        this.worker_id = data().deepCopy(fields()[1].schema(), other.worker_id);
        fieldSetFlags()[1] = true;
      }
      if (isValidValue(fields()[2], other.tags)) {
        this.tags = data().deepCopy(fields()[2].schema(), other.tags);
        fieldSetFlags()[2] = true;
      }
      if (isValidValue(fields()[3], other.state)) {
        this.state = data().deepCopy(fields()[3].schema(), other.state);
        fieldSetFlags()[3] = true;
      }
      if (isValidValue(fields()[4], other.timestamp)) {
        this.timestamp = data().deepCopy(fields()[4].schema(), other.timestamp);
        fieldSetFlags()[4] = true;
      }
    }
    
    /** Creates a Builder by copying an existing Worker instance */
    private Builder(kafka.monitoring.Worker other) {
            super(kafka.monitoring.Worker.SCHEMA$);
      if (isValidValue(fields()[0], other.worker_name)) {
        this.worker_name = data().deepCopy(fields()[0].schema(), other.worker_name);
        fieldSetFlags()[0] = true;
      }
      if (isValidValue(fields()[1], other.worker_id)) {
        this.worker_id = data().deepCopy(fields()[1].schema(), other.worker_id);
        fieldSetFlags()[1] = true;
      }
      if (isValidValue(fields()[2], other.tags)) {
        this.tags = data().deepCopy(fields()[2].schema(), other.tags);
        fieldSetFlags()[2] = true;
      }
      if (isValidValue(fields()[3], other.state)) {
        this.state = data().deepCopy(fields()[3].schema(), other.state);
        fieldSetFlags()[3] = true;
      }
      if (isValidValue(fields()[4], other.timestamp)) {
        this.timestamp = data().deepCopy(fields()[4].schema(), other.timestamp);
        fieldSetFlags()[4] = true;
      }
    }

    /** Gets the value of the 'worker_name' field */
    public java.lang.CharSequence getWorkerName() {
      return worker_name;
    }
    
    /** Sets the value of the 'worker_name' field */
    public kafka.monitoring.Worker.Builder setWorkerName(java.lang.CharSequence value) {
      validate(fields()[0], value);
      this.worker_name = value;
      fieldSetFlags()[0] = true;
      return this; 
    }
    
    /** Checks whether the 'worker_name' field has been set */
    public boolean hasWorkerName() {
      return fieldSetFlags()[0];
    }
    
    /** Clears the value of the 'worker_name' field */
    public kafka.monitoring.Worker.Builder clearWorkerName() {
      worker_name = null;
      fieldSetFlags()[0] = false;
      return this;
    }

    /** Gets the value of the 'worker_id' field */
    public java.lang.CharSequence getWorkerId() {
      return worker_id;
    }
    
    /** Sets the value of the 'worker_id' field */
    public kafka.monitoring.Worker.Builder setWorkerId(java.lang.CharSequence value) {
      validate(fields()[1], value);
      this.worker_id = value;
      fieldSetFlags()[1] = true;
      return this; 
    }
    
    /** Checks whether the 'worker_id' field has been set */
    public boolean hasWorkerId() {
      return fieldSetFlags()[1];
    }
    
    /** Clears the value of the 'worker_id' field */
    public kafka.monitoring.Worker.Builder clearWorkerId() {
      worker_id = null;
      fieldSetFlags()[1] = false;
      return this;
    }

    /** Gets the value of the 'tags' field */
    public java.util.List<java.lang.CharSequence> getTags() {
      return tags;
    }
    
    /** Sets the value of the 'tags' field */
    public kafka.monitoring.Worker.Builder setTags(java.util.List<java.lang.CharSequence> value) {
      validate(fields()[2], value);
      this.tags = value;
      fieldSetFlags()[2] = true;
      return this; 
    }
    
    /** Checks whether the 'tags' field has been set */
    public boolean hasTags() {
      return fieldSetFlags()[2];
    }
    
    /** Clears the value of the 'tags' field */
    public kafka.monitoring.Worker.Builder clearTags() {
      tags = null;
      fieldSetFlags()[2] = false;
      return this;
    }

    /** Gets the value of the 'state' field */
    public java.lang.CharSequence getState() {
      return state;
    }
    
    /** Sets the value of the 'state' field */
    public kafka.monitoring.Worker.Builder setState(java.lang.CharSequence value) {
      validate(fields()[3], value);
      this.state = value;
      fieldSetFlags()[3] = true;
      return this; 
    }
    
    /** Checks whether the 'state' field has been set */
    public boolean hasState() {
      return fieldSetFlags()[3];
    }
    
    /** Clears the value of the 'state' field */
    public kafka.monitoring.Worker.Builder clearState() {
      state = null;
      fieldSetFlags()[3] = false;
      return this;
    }

    /** Gets the value of the 'timestamp' field */
    public java.lang.Long getTimestamp() {
      return timestamp;
    }
    
    /** Sets the value of the 'timestamp' field */
    public kafka.monitoring.Worker.Builder setTimestamp(long value) {
      validate(fields()[4], value);
      this.timestamp = value;
      fieldSetFlags()[4] = true;
      return this; 
    }
    
    /** Checks whether the 'timestamp' field has been set */
    public boolean hasTimestamp() {
      return fieldSetFlags()[4];
    }
    
    /** Clears the value of the 'timestamp' field */
    public kafka.monitoring.Worker.Builder clearTimestamp() {
      fieldSetFlags()[4] = false;
      return this;
    }

    @Override
    public Worker build() {
      try {
        Worker record = new Worker();
        record.worker_name = fieldSetFlags()[0] ? this.worker_name : (java.lang.CharSequence) defaultValue(fields()[0]);
        record.worker_id = fieldSetFlags()[1] ? this.worker_id : (java.lang.CharSequence) defaultValue(fields()[1]);
        record.tags = fieldSetFlags()[2] ? this.tags : (java.util.List<java.lang.CharSequence>) defaultValue(fields()[2]);
        record.state = fieldSetFlags()[3] ? this.state : (java.lang.CharSequence) defaultValue(fields()[3]);
        record.timestamp = fieldSetFlags()[4] ? this.timestamp : (java.lang.Long) defaultValue(fields()[4]);
        return record;
      } catch (Exception e) {
        throw new org.apache.avro.AvroRuntimeException(e);
      }
    }
  }
}
