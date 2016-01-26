<?php

/* We must have all the Tupue php files */
foreach (glob("sites/all/libraries/tuque/*.php") as $filename) {
  require_once $filename;
}

class UTLFedoraObject {

    protected $url;
    protected $user;
    protected $pass;
    protected $repository;
    
    function __construct($user = NULL, $pass = NULL) {
        $this->url = variable_get('islandora_base_url', 'http://localhost:8080/fedora'); 
        $this->user = $user;
        $this->pass = $pass;
        $this->init_connection();
    }

    private function init_connection() {
        $connection = new RepositoryConnection($this->url, $this->user, $this->pass);
        $connection->reuseConnection = TRUE;
        $this->repository = new FedoraRepository(
           new FedoraApi($connection),
           new SimpleCache());
    }
    
    public function get_url() {
        return $this->url;
    }
    
    public function get_object($pid = "") {
        if ($pid != "") {
            try {
                return $this->repository->getObject($pid);
            }catch (Exception $e) {
                return "Error while accessing object: $e\n";
            }
        }
    }

    public function get_children($pid = "") {
        if ($pid != "") {
            try {
                $ri = $this->repository->ri;
                return $ri->sparqlQuery("SELECT *
                    FROM <#ri>
                    WHERE {
                     ?pid <fedora-rels-ext:isMemberOfCollection> <info:fedora/$pid> .
                    }");
            }catch(Exception $e) {
                return "Error while accessing children: $e\n";
            }
        }
    }

    public function get_pages($pid = "") {
        if ($pid != "") {
            try {
                $ri = $this->repository->ri;
                return $ri->sparqlQuery("SELECT *
                    FROM <#ri>
                    WHERE {
                     ?pid <fedora-rels-ext:isMemberOf> <info:fedora/$pid> .
                    }");
            }catch(Exception $e) {
                return "Error while accessing children: $e\n";
            }
        }
    }

	public function add_datastream(&$object, $dsid, $label, $mimetype, $content) {
	//print_r($object);
        if ($object) {
            $datastream = isset($object[$dsid]) ? $object[$dsid] : $object->constructDatastream($dsid);
            $datastream->label = $label;
            $datastream->mimeType = $mimetype;
            $datastream->setContentFromString($content);
            
            // There's no harm in doing this if the datastream is already ingested or if the object is only constructed.
            $object->ingestDatastream($datastream);
            // If the object IS only constructed, ingesting it here also ingests the datastream.
            //$this->repository->ingestObject($object);
        }
    }

}

