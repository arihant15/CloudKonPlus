package kafka.monitoring;

import java.util.ArrayList;
import java.util.List;

import javax.ws.rs.Consumes;
import javax.ws.rs.GET;
import javax.ws.rs.Path;
import javax.ws.rs.QueryParam;
import javax.ws.rs.core.MediaType;
import javax.ws.rs.core.Response;

@Path("/")
public class MRestService {
	@GET
	@Path("/mservice")
	@Consumes(MediaType.APPLICATION_JSON)
	public Response monitoringRest(@QueryParam("name") String name, 
			@QueryParam("workerId") String workerId, @QueryParam("state") String state, 
			@QueryParam("timeStamp") long timestamp, 
			@QueryParam("tags") List<String> t) {
		List<CharSequence> tags = new ArrayList<CharSequence>(t);
		
		Producer producer = Producer.getInstance();
		producer.setSchema(name, workerId, state, timestamp, tags);
		producer.publish();
		
//		System.out.println("Data Received:" + name + "" + tags);
		return Response.status(200).entity("result").build();
	}
}
