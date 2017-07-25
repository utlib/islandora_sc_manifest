<?php
class Canvas {
  protected $object_id;
  protected $pid;
  protected $object_label;
  protected $metadata_uri;
  protected $image_uri;
  protected $image_format;
  protected $image_height;
  protected $image_width;

  private $iiif_base; 

  function __construct($id, $pid, $label, $meta_uri) {
    $this->object_id = $id;
    $this->pid = $pid;
    $this->object_label = $label;
    $this->metadata_uri = $meta_uri;
    $this->iiif_base = variable_get('islandora_sc_manifest_iiif_server', 'https://iiif.library.utoronto.ca/v2/');
  }

  function setImage($thumbnail_uri, $image_uri, $resource_uri, $format = 'image/jpeg', $width, $height) {
    $this->thumbnail_uri = $thumbnail_uri;
    $this->image_uri = $image_uri;
    $this->resource_uri = $resource_uri;
    $this->image_format = $format;
    $this->image_width = $width;
    $this->image_height = $height;
  }

  function toArray() {
    $returnArray = array (
        '@type' => 'sc:Canvas',
        '@id' => $this->object_id,
        'label' => $this->object_label,
        'height' => $this->image_height,
        'width' => $this->image_width,
        'thumbnail' => array (
            '@id' => $this->thumbnail_uri,
            'service' => array (
                '@context' => 'http://iiif.io/api/image/2/context.json',
                '@id' => $this->iiif_base . $this->pid,
                'profile' => 'http://iiif.io/api/image/2/level2.json'
            )
        ),
        'images' => array ( // always contain only one
            array (
                '@id' => $this->image_uri,
                '@type' => 'oa:Annotation',
                'motivation' => 'sc:Painting',
                'on' => $this->object_id,
                'resource' => array (
                    '@id' => $this->resource_uri,
                    '@type' => 'dctypes:Image',
                    'format' => $this->image_format,
                    'height' => $this->image_height,
                    'width' => $this->image_width,
                    'service' => array (
                        '@context' => 'http://iiif.io/api/image/2/context.json',
                        '@id' => $this->iiif_base . $this->pid,
                        'profile' => 'http://iiif.io/api/image/2/level2.json'
                     )
                )
            )
        )
    );

    return $returnArray;
  }
}
