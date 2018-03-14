<?php

class Collection {
  protected $id = '';
  protected $collection_label = '';
  protected $metadata = [];
  protected $metadata_uri = '';
  protected $collections = array ();
  protected $manifests = array ();

  /**
   * Initiate the Collection
   *
   * @param string $id '@id' url for the Collection
   * @param string $collection_label label for the Collection
   * @param string $metadata_uri url for the 'MODS' datastream
   */
  function __construct($id, $collection_label, $metadata_uri) {
    $this->id = $id;
    $this->collection_label = $collection_label;
    $this->metadata_uri = $metadata_uri;
  }

  /**
   * Set the Collection metadata from $metadatas array
   *
   * @param array $metadatas
   */
  function setMetadata($metadatas) {
    foreach($metadatas as $key => $values) {
      $key_values = [];
      foreach($values as $value) $key_values [] = "$value";
      $this->metadata [] = array ('label' => $key, 'value' => $key_values);
    }
  }

  /**
   * Add the Sub Collection to the Collection
   *
   * @param string $collection
   */
  function addCollection($collection = null) {
    if ($collection != null) {
      $this->collections [] = $collection;
    }
  }

  /**
   * Add the Sub manifest to the manifest
   *
   * @param string $manifest
   */
  function addManifest($manifest = null) {
    if ($manifest != null) {
      $this->manifests [] = $manifest;
    }
  }

  /**
   * Build and return a json string based on what we have in the class for the IIIF Collection
   */
  function getJson() {
    $iiif_collection = array (
      '@context' => 'http://iiif.io/api/presentation/2/context.json',
      '@id' => $this->id,
      '@type' => 'sc:Collection',
      'label' => $this->collection_label,
      'metadata' => $this->metadata,
      'attribution' => 'For rights and reproduction information please contact collections@library.utoronto.ca',
      'seeAlso' => array ('@id' => $this->metadata_uri, 'format' => 'application/rdf+xml'),
      'collections' => $this->collections, 
      'manifests' => $this->manifests
    );
    return json_encode ( $iiif_collection );
  }
}
