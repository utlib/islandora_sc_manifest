<?php

/**
* Return an associative array of metadata values for the given object pid
*
* @param string $pid the string PID of the Fedora object
* @param string $dsid the datastream name to read the metadata from
*/
function get_metadatas($pid, $dsid) {
  $fo = new UTLFedoraObject($user->name, $user->pass);
  $api_a = $fo->get_api_a(); // Get Fedora repository's Access API.
  $metadatas = array();
  try {
    $datastream_xml_string = $api_a->getDatastreamDissemination($pid, $dsid);
  } catch (Exception $e) {
    return $metadatas;
  }
  // Get the parsed SimpleXMLElement object.
  $datastream_xml_object = simplexml_load_string($datastream_xml_string);
  $xml = new SimpleXMLIterator($datastream_xml_string);
  $iterator = new RecursiveIteratorIterator($xml);
  foreach ($iterator as $element) {
      $metadata_name = $element->getName();
      // Only grab the 'title' & 'dateIssued' metadata
      if ($metadata_name=='dateIssued' || $metadata_name=='title') {
        if (!isset($metadatas[$metadata_name])) {
          $metadatas[$metadata_name] = array(); // Create new metadata key if it doesn't already exist.
        }
        $metadatas[$metadata_name][] = $element[0]->__toString(); // Append metadata value to existing key. 
      }   
  }
  return $metadatas;
}