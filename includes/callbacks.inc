<?php
/**
 * @file
 * Module callbacks.
 */

function scmanifest_generate($form, &$form_state, $pid) {
  $form['title'] = array(
    '#markup' => 'Generate SC manifest for '.$pid.'<br>'
  );
  $form['object_pid'] = array(
    '#value' =>  $pid->id,
    '#type' => 'hidden'
  );

  if (isset($pid['SC'])) {
    $form['generate'] = array(
      '#type' => 'submit',
      '#value' => t('Re-generate SC Manifest'),
      '#weight' => 1,
      '#submit' => array('generate_submit')
    );
  } else {
      $form['generate'] = array(
        '#type' => 'submit',
        '#value' => t('Generate SC Manifest'),
        '#weight' => 1,
        '#submit' => array('generate_submit')
      );    
  }
  return $form;
}

function generate_submit($form, &$form_state) {
  if (!libraries_load('utlfedoraobject') and !module_load_include('inc', 'islandora_sc_manifest', 'includes/utlfedoraobject')) {
    drupal_set_message(t("UTLFedoraObject not found!"), "error");
  }else {
    module_load_include('inc', 'islandora_sc_manifest', 'includes/utilities');  
    $object_pid = $form_state['values']['object_pid'];
    __generate_iiif_manifest($object_pid);
  }
}
