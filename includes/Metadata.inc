<?php

  /**
  * Return an assoiciative array of metadata values for the given object's datastream
  *
  * @param string $datastream_rdf_xml_string Fedora object's datastream as a RDF-XML string.
  */
  function get_metadatas($datastream_rdf_xml_string) {
    $metadatas = array();
    // Replace all instances of ':' with '__' in datastream_rdf_xml_string, so that we can use simplexml to parse the string.
    // In SimpleXMLElement, node values with ':' is considered invalid.
    $datastream_xml_string = str_replace(':', '__', $datastream_rdf_xml_string);
    // Get the parsed SimpleXMLElement object.
    $datastream_xml_object = simplexml_load_string($datastream_xml_string);
    // Loop through each metadata and add to $metadatas array.
    foreach ($datastream_xml_object as $metadata) {
      $metadata_name = explode("__", $metadata->getName())[1];
      // Only generate the 'date' metadata. UofT Specific Requirements. 
      if ($metadata_name=='date'){
        if (!isset($metadatas[$metadata_name])) {
          $metadatas[$metadata_name] = array(); // Create new metadata key if it doesn't already exist.
        }
        $metadatas[$metadata_name][] = $metadata; // Append metadata value to existing key. 
      }
    }
    return $metadatas;
  }